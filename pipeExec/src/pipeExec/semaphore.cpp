/**
 * @file semaphore.cc
 * @brief This file contains the definition of the Semaphore class, which
 * provides a synchronization mechanism for concurrent access to a shared
 * resource.
 */
#include "semaphore.h"

/**
 * @brief Constructs a new Semaphore object
 *
 * @param count The initial count of the semaphore
 * @param type The type of semaphore we're using (in/out/none)
 * @param debug The flag for debug information
 */
Semaphore::Semaphore(int count, PipeSemaphoreType type, bool debug)
    : count_(count), debug_(debug) {
  switch (type) {
    case PipeSemaphoreType::kNone:
      type_ = "None";
      break;
    case PipeSemaphoreType::kIn:
      type_ = "In";
      break;
    case PipeSemaphoreType::kOut:
      type_ = "Out";
      break;
  }
}

/**
 * @brief Destroys the Semaphore object
 */
Semaphore::~Semaphore() {}

/**
 * @brief Waits for the semaphore
 *
 * This function blocks the calling thread until the semaphore count is
 * greater than or equal to zero.
 */
void Semaphore::Wait() {
  std::unique_lock<std::mutex> lock(mutex_);

  cond_var_.wait(lock, [this] {
    bool result = count_ > 0;
    if (debug_) {
      printf("        (SEMAPHORE %s)%s , count: %d\n", 
             type_.c_str(), 
             result ? "PASSED" : "BLOCKED",
             count_.load() - 1);
    }
    return result;
  });
  count_--;
}

/**
 * @brief Signals the semaphore
 *
 * This function increments the semaphore count and wakes up one waiting
 * thread, if there is any.
 */
void Semaphore::Signal() {
  std::unique_lock<std::mutex> lock(mutex_);
  count_++;
  if (debug_) {
    printf("        (SEMAPHORE %s) SIGNAL count: %d\n", 
           type_.c_str(), count_.load());
  }
  cond_var_.notify_one();
}

/**
 * @brief Returns the current count of the semaphore
 *
 * @return int The current count of the semaphore
 */
int Semaphore::count() { return count_.load(); }

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
