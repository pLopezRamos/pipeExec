#include "sleeper.h"

// TODO: The comment

/**
 * @brief Default constructor: does nothing
 */
Sleeper::Sleeper() {}

/**
 * @brief Default destructor: does nothing
 */
Sleeper::~Sleeper() {}

void Sleeper::Init(void *initData) {
  seconds_to_sleep_ = 0;
  if (initData != nullptr) {
    int* hold = (int*)initData;
//    seconds_to_sleep_ = (int)*hold;
    seconds_to_sleep_ = (int)*(int*)initData;
  }
}

void Sleeper::Run(void *) {
  std::this_thread::sleep_for(std::chrono::seconds(seconds_to_sleep_));
}

ProcessingUnitInterface *Sleeper::Clone() { return new Sleeper; }

void Sleeper::End() {}

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
