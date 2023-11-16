#include "sleeper.h"
#include <iostream>
#include <chrono>
#include "pipeData.h" // Include the necessary header

/**
 * @brief Default constructor: does nothing
 */
Sleeper::Sleeper() { setKey("_#std#sleeper#time#_"); }

/**
 * @brief Default destructor: does nothing
 */
Sleeper::~Sleeper() {}

/**
 * @brief Initialize the time to sleep
 *
 * @details Extract the number of seconds to sleep. If the pointer is null, then the value will be set to 0
 *
 * @param initData - pointer to an integer value.
 *
 * @return void
 */
void Sleeper::Init(void *initData)
{
  seconds_to_sleep_ = 0;

  if (initData != nullptr)
  {
    int *time = static_cast<int *>(initData);
    if (time != nullptr)
    {
      seconds_to_sleep_ = *time;
    }
  }
}

void Sleeper::Run(void *sleepTime)
{ 
  if (sleepTime != nullptr)
  {
    int *time = static_cast<int *>(getExtraData(sleepTime, getKey()));

    if (time == nullptr)
    {
//      std::cout << "from init - sleeping " << seconds_to_sleep_ << "s" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(seconds_to_sleep_));
    }
    else
    {
//      std::cout << "from data - sleeping " << *time << "s" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(*time));
    }
  }
  else
  {
    throw std::runtime_error("Has to be called from pipeExec.");
  }
}
