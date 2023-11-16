#include "pipeline.h"
#include "pipeData.h"

#include <cstdio>
#include <string>
#include <iostream>

/**
 * @brief Constructor for the Pipeline class
 *
 * @details Sets the first Node: assigns the processing unit to the one
 * specified as "first_function" then gets the MemoryManager from the (main)
 * thread and puts it as input and output of the Node. Sets the Node as the last
 * node to know that the data has to be pushed into the IN queue for the
 * circular processing to be working.
 *
 */
Pipeline::Pipeline(ProcessingUnitInterface *first_function, MemoryManager *data_in, int threads_per_node_, bool debug, bool profiling)
  : debug_(debug), show_profiling_(profiling), node_number_(0) {

    PipeNode *first_node = new PipeNode;

    first_node->node_id(node_number_);
    first_node->last_node(true);
    first_node->out_data_queue(data_in);
    first_node->in_data_queue(data_in);
    first_node->processing_unit(first_function);
    first_node->number_of_instances(threads_per_node_);

    execution_list_.push_back(first_node);
    ++node_number_;
  }

/**
 * @brief Default destructor for the pipeline.
 */
Pipeline::~Pipeline() {}

/**
 *
 * @param processing_unit A pointer to an object of ProcessingUnitInterface
 * @param instances The number of threads that have to be instanced to run this
 * node.
 * @param A pointer to a data object
 */
void Pipeline::AddProcessingUnit(ProcessingUnitInterface *processing_unit, int instances, void *startData) {

  PipeNode *new_node = new PipeNode;

  new_node->extra_args(startData);

  int prev_idx = node_number_ - 1;
  execution_list_[prev_idx]->last_node(false);
  execution_list_[prev_idx]->out_data_queue(new MemoryManager(execution_list_[0]->in_data_queue()->max_size(), debug_));

  new_node->out_data_queue(execution_list_[0]->in_data_queue());
  new_node->in_data_queue(execution_list_[prev_idx]->out_data_queue());
  new_node->node_id(node_number_);
  new_node->last_node(true);
  new_node->processing_unit(processing_unit);
  new_node->number_of_instances(instances);
  execution_list_.push_back(new_node);
  ++node_number_;
}

/**
 * @brief The function that all threads execute to run their processing unit.
 * @details This function gets the data from the input MemoryManager, executes
 * the processing unit with the data and sends it through the output queue of
 * the output MemoryManager. If it is the last node it sends the data trhough
 * the input queue of the output MemoryManager.
 *
 * @param node The node to be executed.
 * @param id The thread id. Especially used for debug information.
 * @param mtx The mutex to keep safe the initialization of the node while its
 * cloning instances of the processing unit.
 * @param debug The debug flag
 *
 */
void RunNode(PipeNode *node, int id, std::mutex &mtx, std::mutex &prof, std::vector<Pipeline::Profiling> &profiling_information, bool debug = false, bool profiling = false) {

  void *data = nullptr;
  ProcessingUnitInterface *processing_unit = node->processing_unit();

  if (id != 0) {
    processing_unit = node->processing_unit()->Clone();
    if (processing_unit == nullptr) {
      mtx.unlock();
      throw std::invalid_argument("Clone returned null pointer.");
    }
  }

  mtx.unlock();

  // TIMESTAMP
  if (profiling) {
    prof.lock();
    profiling_information.push_back({.node_id = node->node_id(),
        .thread_id = id,
        .cycles_start = rdtsc(),
        .cycles_end = 0,
        .time_start = STOPWATCH_NOW,
        .time_end = STOPWATCH_NOW,
        .sys_time_start = clock(),
        .sys_time_end = 0});
    prof.unlock();
  }

  // Starts the data at the processing unit, the user must be aware of the arg
  processing_unit->Init(node->extra_args());

  try {
    auto terminate = false;
    do {
      data = node->in_data_queue()->PopFromOut();
      auto pData = (pipeData*)data;
      if ( pData->isKey("_#pipeExecd#node#control#_") ) {
        auto pcmd = static_cast<unsigned int*>(pData->GetExtraData("_#pipeExecd#node#control#_"));
        unsigned int cmd = *pcmd;
        switch ( cmd ) {
          case 0: //std::cout << "Null command received nothing done - cmd = " << cmd << std::endl;
            break;
          case 1: //std::cout << "Increment processing unit instances - cmd = " << cmd << std::endl;
            mtx.lock();
            *pcmd = 0; // Set the command to the null command
            node->PushThread(new std::thread(RunNode, node, node->number_of_instances(), std::ref(mtx), std::ref(prof), std::ref(profiling_information), debug, profiling));
            node->number_of_instances(node->number_of_instances() + 1);
            std::cout << "NODE " << node->node_id() << " NUMBER OF INSTANCES = " << node->number_of_instances() << std::endl;
            break;
          case 2: std::cout << "NODE = " << node->node_id() << " Decrement processing unit instances - cmd = " << cmd << " NUMBER OF INSTANCES = " << node->number_of_instances() << std::endl;
                  mtx.lock();
                  *pcmd = 0;
                  if ( node->number_of_instances() > 1 ) { terminate = true;
                    node->number_of_instances(node->number_of_instances() - 1);
                  }
                  else std::cout << "LAST THREAD - IGNORING TERMINATION " << std::endl;
                  mtx.unlock();
                  break;
          default: std::cout << "Command id " << cmd << "not implemented." << std::endl;
        }
      }

      pData->setNodeData(node);
      // Runs the processing_unit
      processing_unit->Run(data);

      if (node->last_node()) {
        node->out_data_queue()->PushIntoIn(data);
      } else {
        node->out_data_queue()->PushIntoOut(data);
      }
      if (profiling) {
        prof.lock();
        for (auto& info : profiling_information) {
          if (info.node_id == node->node_id() && info.thread_id == id) {
            info.cycles_end = rdtsc();
            info.time_end = STOPWATCH_NOW;
            info.sys_time_end = clock();
          }
        }
        prof.unlock();
      }
    } while ( ! terminate );
    std::cout << "NODE " << node->node_id() << " NUMBER OF INSTANCES = " << node->number_of_instances() << std::endl;
  } catch (...) {
  }
  processing_unit->End();
}

/**
 * @brief Sets the pipeline to run.
 * @details For each node inside the execution list it creates "n" instances of
 * threads per node and executes all of them.
 *
 * @return The number of nodes executed
 */
int Pipeline::RunPipe() {
  int nodes_executed = 0;
  for (int it = 0; it < node_number_; ++it) {
    int number_of_subthreads = execution_list_[it]->number_of_instances();
    for (int thread_it = 0; thread_it < number_of_subthreads; ++thread_it) {
      try {
        execution_mutex_.lock();
        execution_list_[it]->PushThread(new std::thread(
              RunNode, execution_list_[it], thread_it, std::ref(execution_mutex_),
              std::ref(profiling_mutex_), std::ref(profiling_list_), debug_,
              show_profiling_));
      } catch (...) {
      }
    }
    ++nodes_executed;
  }
  return nodes_executed;
}

/**
 * @brief Waits until all the threads have finished putting its data inside the
 * (main)'s in_queue MemoryManager.
 */
void Pipeline::WaitFinish() {
  while (execution_list_[0]->in_data_queue()->in_queue_count() !=
      execution_list_[0]->in_data_queue()->max_size()) {
    execution_list_[0]->in_data_queue()->wait_finish();
  }
}

void Pipeline::Profile() {
  std::sort(profiling_list_.begin(), profiling_list_.end(),
      [](const Pipeline::Profiling &a, const Pipeline::Profiling &b) {
      return a.node_id <= b.node_id && a.thread_id <= b.thread_id;
      });

  for (Profiling profile : profiling_list_) {
    printf(
        "sNODE %d\t THREAD %d\n    Time running: %ldms\n    Cycles since "
        "init of run: %lu\n    System time: % fms\n ",
        profile.node_id, profile.thread_id,
        TIME_IN_MS(profile.time_start, profile.time_end),
        profile.cycles_end - profile.cycles_start,
        ((double)(profile.sys_time_end - profile.sys_time_start) /
         CLOCKS_PER_SEC) *
        1000);
  }
}
