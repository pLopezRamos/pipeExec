#include "cube.h"

#include <cstdio>
#include <string>
#include <iostream>

/**
 * @brief Default destructor for the Cube.
 */
Cube::~Cube()
{

  PipeNode *node;

  for (auto x = 0; x < xRange_; ++x)
  {
    for (auto y = 0; y < yRange_; ++y)
    {
      for (auto z = 0; z < zRange_; ++z)
      {
        node = (PipeNode *)threeDimPipe->getPipeNode(pipeMapper::nodeId(x, y, 0));
        delete (node);
      }
    }
  }
}

/**
 * @brief Constructor for the Cube class
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
Cube::Cube(unsigned int xRange, unsigned int yRange, unsigned int zRange, unsigned int queueSize, bool closed)
{

  threeDimPipe = new pipeMapper();

  PipeNode *new_node;

  for (auto x = 0; x < xRange; ++x)
  {
    for (auto y = 0; y < yRange; ++y)
    {
      for (auto z = 0; z < zRange; ++z)
      {
        new_node = new PipeNode;
        new_node->extra_args(nullptr);
        new_node->ctl_sema = new Semaphore(0);
        new_node->node_id(node_number_);
        new_node->last_node(false);
        new_node->processing_unit(nullptr);
        new_node->number_of_instances(1);
        new_node->max_instances(1);
        new_node->min_instances(1);
        new_node->in_data_queue(new pipeQueue(queueSize, false));
        new_node->setNodeAddress(threeDimPipe->addNode(new_node, "", pipeMapper::nodeId(x, y, z)));
        ++node_number_;
      }
      new_node->last_node(true);
    }
  }
  xRange_ = xRange;
  yRange_ = yRange;
  zRange_ = zRange;

  out_queue(new pipeQueue(queueSize));
}

/**
 * @brief Adda processing unit to the node
 *
 * @details Add a processsing unit and its attributes a the node defined by its x and y
 * coordinates
 *
 * @param procUnit a pointer to a processing unit object.
 * @param instances number of instances of the processing unit to create
 * @param initData data to be passed to the Init() method of the processing unit
 * @param queueSize the size of the output queue
 * @param maxInstances the maximun number of instances that can be reached when dinamicaly increased.
 * @param minInstances the minimum number of instances that can be left when dinamicaly decreasing.
 * @param x the x coordinate of the node
 * @param y the y coordinate of the node
 *
 * @returns a pointer to the node.
 *
 * @throws std::invalid_argument
 */
PipeNode *Cube::AddProcessingUnit(ProcessingUnitInterface *procUnit, int instances, unsigned int x, unsigned int y, unsigned int z, pipeData::dataPacket initData, int maxInstances, int minInstances, unsigned int queueSize)
{

  if (x > xRange_ || y > yRange_ || z > zRange_)
  {
    throw std::invalid_argument("address out of range.");
  }

  auto node = (PipeNode *)threeDimPipe->getPipeNode(pipeMapper::nodeId(x, y, z));

  node->extra_args(initData);
  if (node->processing_unit() != nullptr)
  {
    node->processing_unit()->End();
  }
  node->processing_unit(procUnit);
  node->number_of_instances(instances);
  node->max_instances(maxInstances);
  node->min_instances(minInstances);
  if (queueSize != 0)
  {
    delete (node->in_data_queue());
    node->in_data_queue(new pipeQueue(queueSize, false));
  }

  return node;
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
 *
 */
void RunNode(PipeNode *node, int n_id, std::mutex &mtx, pipeMapper *map, pipeQueue *outQueue)
{

  ProcessingUnitInterface *processing_unit = node->processing_unit();

  // std::cout << __func__ << " : " << __LINE__ << std::endl;
  if (n_id != 0)
  {
    processing_unit = node->processing_unit()->Clone();
    if (processing_unit == nullptr)
    {
      mtx.unlock();
      throw std::invalid_argument("Clone returned null pointer.");
    }
  }

  mtx.unlock();

  // std::cout << __func__ << " : " << __LINE__ << std::endl;
  //  Starts the data at the processing unit, the user must be aware of the arg
  processing_unit->Init(node->extra_args());

  try
  {
    // std::cout << __func__ << " : " << __LINE__ << std::endl;
    auto terminate = false;
    do
    {
      // std::cout << "NODE " << node->node_id() << " RUNNING INST " << n_id << " OF " << node->number_of_instances() << std::endl;
      auto data = node->in_data_queue()->Pop();
      auto pnode = (PipeNode *)map->getPipeNode(node->getPrevAddress());
      if (pnode != node)
      {
        pnode->ctl_mtx.lock();
        auto cmd = pnode->getCmd();
        node->ctl_mtx.lock();
        while (cmd != PipeNode::nodeCmd::EMPTY)
        {
          switch (cmd)
          {
          case PipeNode::nodeCmd::NO_OP:
            std::cout << "Null command received nothing done - cmd = " << cmd << std::endl;
            break;
          case PipeNode::nodeCmd::ADD_THR:
            std::cout << "Increment processing unit instances - cmd = " << cmd << std::endl;
            if (node->max_instances() == 0 || node->max_instances() > node->number_of_instances())
            {
              mtx.lock();
              std::cout << "NODE " << node->node_id() << " LAUNCH NEW INSTANCE " << std::endl;
              node->PushThread(new std::thread(RunNode, node, node->number_of_instances(), std::ref(mtx), std::ref(map), std::ref(outQueue)));
              node->number_of_instances(node->number_of_instances() + 1);
            }
            break;
          case PipeNode::nodeCmd::END_THR:
            std::cout << "Decrement processing unit instances - cmd = " << cmd << std::endl;
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

      // std::cout << "NODE " << node->node_id() << " RUNNING INST " << n_id << " OF " << node->number_of_instances() << std::endl;

      if (node->processing_unit() != processing_unit)
      {
        processing_unit->End(data);
        processing_unit = node->processing_unit();
        processing_unit->Init(node->extra_args());
      }

      auto pData = (pipeData *)data;
      pData->setNodeData(node);

      // std::cout << "NODE " << node->node_id() << " START RUN " << std::endl;
      //  Runs the processing_unit
      processing_unit->Run(data);
      // std::cout << "NODE " << node->node_id() << " END   RUN " << std::endl;

      // Check if the proccesing unit wants to write to a named address
      auto namedNode = (std::string *)pData->GetExtraData("_#NAMED_ADDRESS#_");
      PipeNode *nextNode;
      pipeMapper::nodeId *nextNodeId = nullptr;
      if (namedNode != nullptr)
      {
        // If the address is WRITE_OUT then write to the output queue
        if (*namedNode == "_#WRITE_OUT#_")
        {
          outQueue->Push(data);
        }
        else
        {
          // If no, get the node associated with the address
          nextNode = (PipeNode *)map->getPipeNode(*namedNode);
        }
      }
      else
      {
        // If the address is NEXT_ADDRESS, you get a nodeId else you get nullptr
        nextNodeId = (pipeMapper::nodeId *)pData->GetExtraData("_#NEXT_ADDRESS#_");
      }

      if (nextNodeId != nullptr)
      {
        // Check that the node exists
        if (map->nodeExists(*nextNodeId))
        {
          // Get the node assiated with the address
          auto next_node = (PipeNode *)map->getPipeNode((pipeMapper::nodeId)*nextNodeId);
          next_node->in_data_queue()->Push(data);
        }
      }
      else
      {
        // If not address given, just go to the next node
        auto id = node->getNodeAddress();
        if (node->last_node())
        {
          outQueue->Push(data);
        }
        else
        {
          id.z += 1;
          // std::cout << "NODE id.x = " << id.x << " id.y = " << id.y << std::endl;
          auto next_node = (PipeNode *)map->getPipeNode(id);
          next_node->in_data_queue()->Push(data);
        }
      }
      // std::cout << "NODE " << node->node_id() << " DATA PUSHED " << std::endl;

      if (terminate)
        processing_unit->End(data);
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
int Cube::RunCube()
{

  int nodes_executed = 0;
  auto id = pipeMapper::nodeId(0, 0, 0);
  PipeNode *node;
  bool done = false;

  for (int x = 0; x < xRange_; ++x)
  {
    for (int y = 0; y < yRange_; ++y)
    {
      for (int z = 0; z < zRange_; ++z)
      {
        node = (PipeNode *)threeDimPipe->getPipeNode(pipeMapper::nodeId(x, y, 0));
        auto numberOfInstances = node->number_of_instances();
        for (auto instanceIt = 0; instanceIt < numberOfInstances; ++instanceIt)
        {
          try
          {
            execution_mutex_.lock();
            node->PushThread(new std::thread(
                RunNode, node, instanceIt, std::ref(execution_mutex_),
                threeDimPipe, std::ref(out_queue_)));
          }
          catch (...)
          {
          }
        }
        ++nodes_executed;
      }
    }
  }
  return nodes_executed;
}