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
  lastCount_ = 2;
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
  auto currentCount = outQueue->out_queue_count();
  auto cmd = Pipeline::nodeCmd::NO_OP;

  if ( currentCount >= lastCount_  ) {
    if ( adaptable_ && (currentCount - lastCount_) > 0 ) {
      cmd = Pipeline::nodeCmd::ADD_THR;
    } else if ( (currentCount - lastCount_ ) == 0 )
      --currentCount;  // If now equal,  make sure to increment if next equal
  } else if ( currentCount == 0 && adaptable_ ) {
    cmd = Pipeline::nodeCmd::END_THR;
  }

  if ( cmd != Pipeline::nodeCmd::NO_OP ) {
    if ( ! pdata->isKey(getKey()) ) {
      pdata->setDataKey(getKey(), (void*)new int(static_cast<int>(cmd)));
    } else {
      auto *pcmd = (Pipeline::nodeCmd*)pdata->GetExtraData(getKey());
      *pcmd = cmd;
    }
  }

  lastCount_ = currentCount;
}
