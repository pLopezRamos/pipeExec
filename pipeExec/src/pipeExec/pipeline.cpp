#include "pipeline.h"

#include <cstdio>
#include <string>

//                              |  ...  |
//                              |-------|
//                              | data  |
//                              |-------|
//                              |   |   |
//                              |   v   |
// |-----|-----|-----|      +----------------+
// | ... | PU  |  PU |  ->  |    PipeNode    |
// |-----|-----|-----|      +----------------+

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
Pipeline::Pipeline(ProcessingUnitInterface *first_function,
                   MemoryManager *data_in, int threads_per_node_, bool debug,
                   bool profiling)
    : debug_(debug), show_profiling_(profiling) {
  node_number_ = 0;
  PipeNode *first_node;

  // first_node = (PipeNode *)malloc(sizeof(PipeNode));
  first_node = new PipeNode;
  first_node->node_id(node_number_);
  first_node->last_node(true);
  first_node->out_data_queue(data_in);
  first_node->in_data_queue(data_in);
  first_node->processing_unit(first_function);
  first_node->number_of_instances(threads_per_node_);

  execution_list_.push_back(first_node);
  node_number_ += 1;
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
void Pipeline::AddProcessingUnit(ProcessingUnitInterface *processing_unit,
                                 int instances, void *startData) {
  PipeNode *new_node = new PipeNode;

  new_node->extra_args(startData);

  int prev_idx = node_number_ - 1;
  execution_list_[prev_idx]->last_node(false);
  execution_list_[prev_idx]->out_data_queue(new MemoryManager(
      execution_list_[0]->in_data_queue()->max_size(), debug_));

  new_node->out_data_queue(execution_list_[0]->in_data_queue());
  new_node->in_data_queue(execution_list_[prev_idx]->out_data_queue());
  new_node->node_id(node_number_);
  new_node->last_node(true);
  new_node->processing_unit(processing_unit);
  new_node->number_of_instances(instances);
  execution_list_.push_back(new_node);
  node_number_ += 1;
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
void RunNode(PipeNode *node, int id, std::mutex &mtx, std::mutex &prof,
             std::vector<Pipeline::Profiling> &profiling_information,
             bool debug = false, bool profiling = false) {
  try {
    void *data;
    ProcessingUnitInterface *processing_unit;
    processing_unit = node->processing_unit();
    if (id != 0) {
      processing_unit = node->processing_unit()->Clone();
    }
    if (processing_unit == nullptr) {
      fprintf(stderr,
              "(NODE %d)(THREAD %d): Cannot clone the processing "
              "unit: GOING TO SET AS THE DEFAULT GIVEN BY THE NODE\n",
              node->node_id(), id );
      processing_unit = node->processing_unit();
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

    do {
      if (debug) {
        printf("s(NODE %d)(THREAD %d): Popping from IN->OUT\n",
               node->node_id(), id);
      }
      data = node->in_data_queue()->PopFromOut();

      // Runs the processing_unit
      processing_unit->Run(data);

      if (node->last_node()) {
        if (debug) {
          printf("(NODE %d)(THREAD %d): Pushing into OUT->IN\n",
                 node->node_id(), id);
        }
        node->out_data_queue()->PushIntoIn(data);
      } else {
        if (debug) {
          printf("(NODE %d)(THREAD %d): Pushing into OUT->OUT\n",
                 node->node_id(), id);
        }
        node->out_data_queue()->PushIntoOut(data);
      }
      if (profiling) {
        prof.lock();
        uint64_t idx = 0;
        for (uint64_t i = 0; i < profiling_information.size(); ++i) {
          if (profiling_information[i].node_id == node->node_id() &&
              profiling_information[i].thread_id == id) {
            profiling_information[i].cycles_end = rdtsc();
            profiling_information[i].time_end = STOPWATCH_NOW;
            profiling_information[i].sys_time_end = clock();
          }
        }
        prof.unlock();
      }
    } while (true);

    // ############################## PROFILING

    // #########################################

    processing_unit->End();
  } catch (...) {
  }
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
    nodes_executed += 1;
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
  };
}

/**
 * @brief A private function that couns the chars into const char*
 */
int Pipeline::count_arguments(const char *str) {
  int counter = 0;
  while (str[counter] != '\0') {
    counter += 1;
  }
  return counter;
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

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
