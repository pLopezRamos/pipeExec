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
void Drano::Init(pipeData::dataPacket initData)
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

void Drano::Run(pipeData::dataPacket data)
{ 

  auto pdata = (pipeData*)data;
  auto node = pdata->getNodeData();
//  auto outQueue = node->out_data_queue();
  auto inQueue = node->in_data_queue();
  auto inCount = inQueue->queue_count();
//  auto outCount = outQueue->queue_count();
  auto maxSize = outQueue->max_size();
  auto cmd = PipeNode::nodeCmd::NO_OP;

 std::cout << " node id = " << node->node_id() <<" IN = " << inCount;

  if ( inCount ) cmd = PipeNode::nodeCmd::ADD_THR;
  else if ( adaptable_ ) {
      if ( outCount < inCount ) cmd = PipeNode::nodeCmd::END_THR;
  }

  if ( cmd != PipeNode::nodeCmd::NO_OP ) node->setCmd(cmd);

  std::cout << " cmd = " << cmd << std::endl;

  lastCount_ = outCount;
}
