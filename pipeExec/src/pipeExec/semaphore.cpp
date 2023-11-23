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
Semaphore::Semaphore(int count) : count_(count) { }

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
  cond_var_.notify_one();
}

/**
 * @brief Returns the current count of the semaphore
 *
 * @return int The current count of the semaphore
 */
int Semaphore::count() const { return count_.load(); }

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
