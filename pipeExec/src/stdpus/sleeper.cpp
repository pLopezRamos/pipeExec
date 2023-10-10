#include "sleeper.h"
#include <iostream>

// TODO: The comment

/**
 * @brief Default constructor: does nothing
 */
Sleeper::Sleeper() {}

/**
 * @brief Default destructor: does nothing
 */
Sleeper::~Sleeper() {}

/**
 * @brief Initalize the time to sleep
 * 
 * @details Extract the number of seconds to sleep. If the
 * pointer is null, then the value will be set to 0
 *
 * @param initData - pointer to an integer value.
 *
 * @return void
 */

void Sleeper::Init(void *initData) {
  seconds_to_sleep_ = 0;
  if (initData != nullptr) {
    seconds_to_sleep_ = (int)*(int*)initData;
  }
}

void Sleeper::Run(void *sleepTime) {

  int time = (int)*(int*)sleepTime;

  if ( time > 0 ) {
//    std::cout << "from data - sleeping " << time << "s" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(time));
  } else {
//    std::cout << "flom init - sleeping " << seconds_to_sleep_ << "s" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(seconds_to_sleep_));
    }
    
}

ProcessingUnitInterface *Sleeper::Clone() { return new Sleeper; }

void Sleeper::End() {}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
