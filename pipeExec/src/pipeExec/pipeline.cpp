#include "pipeline.h"

#include <cstdio>
#include <string>
#include <iostream>

/**
 * @brief Constructor for the Pipeline class
 *
 * @details Sets the first node for the execution list
 *
 * @param procUnit a pointer to a processing unit object.
 * @param data_in pointer to the imput queue
 * @param data_out pointer to the output queue
 * @param instances number of instances of the processing unit to create
 * @param initData data to be passed to the Init() method of the processing unit
 *
 */
Pipeline::Pipeline(ProcessingUnitInterface *procUnit, pipeQueue *data_in, pipeQueue *data_out, int instances, pipeData::dataPacket initData, bool debug, bool profiling)
    : debug_(debug), show_profiling_(profiling), node_number_(0)
{

  PipeNode *first_node = new PipeNode;

  first_node->extra_args(initData);
  first_node->ctl_sema = new Semaphore(0);
  first_node->node_id(node_number_);
  first_node->last_node(true);
  first_node->out_data_queue(data_out);
  first_node->in_data_queue(data_in);
  first_node->processing_unit(procUnit);
  first_node->number_of_instances(instances);
  first_node->setPrev(first_node);
  first_node->setNext(first_node);
  firstNode_ = first_node;
  lastNode_ = first_node;

//    execution_list_.push_back(first_node);
  ++node_number_;
}

/**
 * @brief Default destructor for the pipeline.
 */
Pipeline::~Pipeline() {}

/**
 * @brief Add a new node to the execution list.
 *
 * @details Creates a new node by, among other things, linking the node with the previous node, setting as the imput queue
 * the previous node output queue, creating the node output queue and adding the node to the execution list.
 *
 * @param procUnit a pointer to a processing unit object.
 * @param instances number of instances of the processing unit to create
 * @param initData data to be passed to the Init() method of the processing unit
 * @param queueSize the size of the output queue
 * @param maxInstances the maximun number of instances that can be reached when dinamicaly increased.
 * @param minInstances the minimum number of instances that can be left when dinamicaly decreasing.
 *
 * @returns a pointer to the node.
 */
PipeNode *Pipeline::AddProcessingUnit(ProcessingUnitInterface *procUnit, int instances, pipeData::dataPacket initData, int queueSize, int maxInstances, int minInstances)
{

  PipeNode *new_node = new PipeNode;

  new_node->extra_args(initData);

  int prev_idx = node_number_ - 1;
  lastNode_->last_node(false);
  new_node->ctl_sema = new Semaphore(0);
  new_node->node_id(node_number_);
  new_node->last_node(true);
  new_node->processing_unit(procUnit);
  new_node->number_of_instances(instances);
  new_node->max_instances(maxInstances);
  new_node->min_instances(minInstances);
  new_node->setPrev(lastNode_);
  new_node->getPrev()->setNext(new_node);
  new_node->setNext(nullptr);
  new_node->in_data_queue(new_node->getPrev()->out_data_queue());
  new_node->out_data_queue(new pipeQueue(queueSize, debug_));
//  execution_list_.push_back(new_node);
  lastNode_ = new_node;
  ++node_number_;
  return new_node;
}

/**
 * @brief Inserts a new node to the execution list.
 *
 * @details Inserts a new node after the given node by, among other things, linking the node with the previous node, setting as the imput queue
 * the previous node output queue, creating the node output queue and adding the node to the execution list.
 *
 * @param pNode a pointer to the previous node.
 * @param procUnit a pointer to a processing unit object.
 * @param instances number of instances of the processing unit to create
 * @param initData data to be passed to the Init() method of the processing unit
 * @param queueSize the size of the output queue
 * @param maxInstances the maximun number of instances that can be reached when dinamicaly increased.
 * @param minInstances the minimum number of instances that can be left when dinamicaly decreasing.
 *
 * @returns a pointer to the node.
 */
PipeNode *Pipeline::InsertProcessingUnit(PipeNode *pNode, ProcessingUnitInterface *procUnit, int instances, pipeData::dataPacket initData, int queueSize, int maxInstances, int minInstances)
{
  return nullptr;
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
void RunNode(PipeNode *node, int id, std::mutex &mtx, std::mutex &prof, std::vector<Pipeline::Profiling> &profiling_information, bool debug = false, bool profiling = false)
{

  ProcessingUnitInterface *processing_unit = node->processing_unit();

  // std::cout << "In function " << __func__ << " line " << __LINE__ << std::endl;

  if (id != 0)
  {
    processing_unit = node->processing_unit()->Clone();
    if (processing_unit == nullptr)
    {
      mtx.unlock();
      throw std::invalid_argument("Clone returned null pointer.");
    }
  }

  mtx.unlock();

  // TIMESTAMP
  if (profiling)
  {
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

  try
  {
    auto terminate = false;
    do
    {
      auto data = node->in_data_queue()->Pop();
      auto pnode = node->getPrev();
      if (pnode != node)
      {
        pnode->ctl_mtx.lock();
        auto cmd = pnode->getCmd();
        node->ctl_mtx.lock();
        while (cmd != PipeNode::nodeCmd::EMPTY)
        {
          switch (cmd)
          {
          case PipeNode::nodeCmd::NO_OP: // std::cout << "Null command received nothing done - cmd = " << cmd << std::endl;
            break;
          case PipeNode::nodeCmd::ADD_THR: //  std::cout << "Increment processing unit instances - cmd = " << cmd << std::endl;
            if (node->max_instances() == 0 || node->max_instances() > node->number_of_instances())
            {
              mtx.lock();
              std::cout << "NODE " << node->node_id() << " LAUNCH NEW INSTANCE " << std::endl;
              node->PushThread(new std::thread(RunNode, node, node->number_of_instances(), std::ref(mtx), std::ref(prof), std::ref(profiling_information), debug, profiling));
              node->number_of_instances(node->number_of_instances() + 1);
            }
            break;
          case PipeNode::nodeCmd::END_THR: // std::cout << "Decrement processing unit instances - cmd = " << cmd << std::endl;
            if (node->min_instances() == 0 || node->min_instances() < node->number_of_instances())
            {
              if (node->number_of_instances() > 1)
              {
                std::cout << "NODE " << node->node_id() << " REMOVING INSTANCE " << std::endl;
                terminate = true;
                node->number_of_instances(node->number_of_instances() - 1);
              }
            }
            break;
          default:
            std::cout << "Command id " << cmd << "not implemented." << std::endl;
          }
          cmd = pnode->getCmd();
        }
        node->ctl_mtx.unlock();
        pnode->ctl_mtx.unlock();
      }

      std::cout << "NODE " << node->node_id() << " RUNNING INST " << id << " OF " << node->number_of_instances() << std::endl;

      //      void *data = nullptr;
      auto pData = (pipeData *)data;
      pData->setNodeData(node);

      //      std::cout << "NODE " << node->node_id() << " START RUN " << std::endl;
      // Runs the processing_unit
      processing_unit->Run(data);
      //      std::cout << "NODE " << node->node_id() << " END   RUN " << std::endl;

      node->out_data_queue()->Push(data);
      //      std::cout << "NODE " << node->node_id() << " DATA PUSHED " << std::endl;

      // std::cout << "In function " << __func__ << " line " << __LINE__ << std::endl;
      if (terminate)
        processing_unit->End(data);

      if (profiling)
      {
        prof.lock();
        for (auto &info : profiling_information)
        {
          if (info.node_id == node->node_id() && info.thread_id == id)
          {
            info.cycles_end = rdtsc();
            info.time_end = STOPWATCH_NOW;
            info.sys_time_end = clock();
          }
        }
        prof.unlock();
      }
    } while (!terminate);
  }
  catch (...)
  {
  }
}

/**
 * @brief Sets the pipeline to run.
 * @details For each node inside the execution list it creates "n" instances of
 * threads per node and executes all of them.
 *
 * @return The number of nodes executed
 */
int Pipeline::RunPipe()
{
  int nodes_executed = 0;
  auto node = firstNode_;
  bool done = false;

  do
  {
    auto numberOfInstances = node->number_of_instances();
    for (auto instanceIt = 0; instanceIt < numberOfInstances; ++instanceIt)
    {
      try
      {
        execution_mutex_.lock();
        node->PushThread(new std::thread(
            RunNode, node, instanceIt, std::ref(execution_mutex_),
            std::ref(profiling_mutex_), std::ref(profiling_list_), debug_,
            show_profiling_));
      }
      catch (...)
      {
      }
    }
    ++nodes_executed;

    done = node->last_node();
    node = node->getNext();
  } while (!done);
  /*  for (int it = 0; it < node_number_; ++it)
    {
      int number_of_subthreads = execution_list_[it]->number_of_instances();
      for (int thread_it = 0; thread_it < number_of_subthreads; ++thread_it)
      {
        try
        {
          execution_mutex_.lock();
          execution_list_[it]->PushThread(new std::thread(
              RunNode, execution_list_[it], thread_it, std::ref(execution_mutex_),
              std::ref(profiling_mutex_), std::ref(profiling_list_), debug_,
              show_profiling_));
        }
        catch (...)
        {
        }
      }
      ++nodes_executed;
    }*/
  return nodes_executed;
}

void Pipeline::Profile()
{
  std::sort(profiling_list_.begin(), profiling_list_.end(),
            [](const Pipeline::Profiling &a, const Pipeline::Profiling &b)
            {
              return a.node_id <= b.node_id && a.thread_id <= b.thread_id;
            });

  for (Profiling profile : profiling_list_)
  {
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
