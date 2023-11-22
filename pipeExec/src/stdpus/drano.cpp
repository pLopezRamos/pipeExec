#include "drano.h"

/**
 * @brief Default constructor: does nothing
 */
Drano::Drano() { setKey("_#pipeExecd#node#control#_"); }

/**
 * @brief Default destructor: does nothing
 */
Drano::~Drano() {}

/**
 * @brief Initialize the time to sleep
 *
 * @details Extract the number of seconds to sleep. If the pointer is null, then the value will be set to 0
 *
 * @param initData - pointer to an integer value.
 *
 * @return void
 */
void Drano::Init(void *initData)
{
  lastCount_ = 0;
  ignoreCount_ = 0;
  adaptable_ = false;

  if (initData != nullptr)
  {
    auto adaptable = static_cast<bool *>(initData);
    if (adaptable != nullptr)
    {
      adaptable_ = *adaptable;
    }
  }
}

void Drano::Run(void *data)
{ 

  auto pdata = (pipeData*)data;
  auto node = pdata->getNodeData();
  auto outQueue = node->out_data_queue();
  auto currentCount = outQueue->queue_count();
  auto maxSize = outQueue->max_size();
  auto cmd = PipeNode::nodeCmd::NO_OP;

  std::cout << " node id = " << node->node_id() <<" currentCount = " << currentCount << " lastCount_ " << lastCount_;

  if ( currentCount > lastCount_ ) {
    cmd = PipeNode::nodeCmd::ADD_THR;
    ignoreCount_ = currentCount;
  } else if ( (currentCount <= lastCount_) && adaptable_ )
    cmd = PipeNode::nodeCmd::END_THR;

  std::cout << " SENDING CMD = " << cmd << " ";
  if ( cmd != PipeNode::nodeCmd::NO_OP ) {
      node->setCmd(cmd);
  }

  lastCount_ = currentCount;
}
