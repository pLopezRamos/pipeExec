/*
 * pipeExec is a library for creating concurrent proccesing pipes
 *
 * Copyright (C) 2023 Lucas Hernández Abreu and Pablo López Ramos
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author:  Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

/**
 * @file pipeQueue.cc
 *
 * @brief The source file for the pipeQueue class
 * @details Here all the methods of the class are defined along with the
 * security and error handling for the class
 *
 * @author Pablo López Ramos
 *
 * Contact: lopez.ramos.pablo@gmail.com
 */

#include "pipeQueue.h"
#include <malloc.h>
#include <cstdio>

/**
 * @brief Constructor for pipeQueue class
 *
 * @param mx_size The maximum number of memory buffers the queue can hold
 * @param debug The flag for the debug information
 * @throw invalid_argument If the maximum size is less than 1
 */
pipeQueue::pipeQueue(int mx_size, bool debug)
  : max_size_(mx_size), debug_(debug), rear_iterator_(-1), 
  front_iterator_(0), queue_count_(0) {
    // Validate the maximum size parameter
    if (mx_size < 1) {
      throw std::invalid_argument("mx_size has to be grater 0");
    }

    // Allocate memory for queue_ and out_queue_ using malloc
    queue_ = (void**)malloc(max_size_ * sizeof(void*));

    // Initialize all elements of queue_ to nullptr using a loop
    for (int it = 0; it < max_size_; ++it) {
      queue_[it] = nullptr;
    }

    // These queue_semaphores are initialized with counts set to 0
    // and a specified type (kIn or kOut) for synchronization
    pop_semaphore_ = new Semaphore(0);
    push_semaphore_ = new Semaphore(max_size_);
  }

/**
 * @brief Destructor for pipeQueue.
 * @details Frees the buffers inside both queues and then frees the queues
 */
pipeQueue::~pipeQueue() {
  // Free the buffers inside the queue_ and out_queue_ arrays
  for (int it = 0; it < max_size_; ++it) {
    free(queue_[it]);
  }

  // Free both arrays
  free(queue_);
}

/**
 * @brief Pushes a memory buffer into the input queue.
 *
 * @param data Pointer to the memory buffer.
 *
 * @return True if the input queue is not full, false otherwise.
 */
 bool pipeQueue::Push(void *data) {
  push_semaphore_->Wait();
  // Acquire the lock for the queue_mutex_
  // This ensures that only one thread can access the queue_ array at a time
  push_mutex_.lock();

  // Check if the queue is full
 /* if (queue_count_ == max_size_) {

    // The queue is full, so we cannot push the data into the queue
    push_mutex_.unlock();
    return false;
  }*/

  // Push the data into the queue
  rear_iterator_ += 1;
  queue_[rear_iterator_] = data;

  // If the rear iterator has reached the end of the array, wrap it around to the beginning
  if (rear_iterator_ == max_size_ - 1) {
    rear_iterator_ = -1;
  }

  // Increment the queue_count_
  queue_count_ += 1;

  // Signal the queue_semaphore_ queue_semaphore to wake up a thread that is waiting to pop an element from the queue_
  pop_semaphore_->Signal();

  // Release the lock for the queue_mutex_
  push_mutex_.unlock();

  // Return true to indicate that the data was successfully pushed into the queue
  return true;
}

/**
 * @brief Pops a memory buffer from the input queue.
 *
 * @throw Throws out_of_range If the content to return is null
 * (it can't be processed)
 *
 * @return Pointer to the memory buffer.
 */
void *pipeQueue::Pop(bool block) {
  
  if ( (queue_count_ == 0) && ! block ) return nullptr;

  // Wait for the queue_semaphore_ queue_semaphore to be signaled, indicating that there is an element in the queue_
  pop_semaphore_->Wait();

  // Acquire the lock for the queue_mutex_
  // This ensures that only one thread can access the queue_ array at a time
  pop_mutex_.lock();

  // Decrement the queue_count_
  queue_count_ -= 1;

  // Pop the element from the queue
  void *memory_buffer = queue_[front_iterator_];
  queue_[front_iterator_] = nullptr;

  // Increment the front iterator
  front_iterator_ = (front_iterator_ + 1) % max_size_;

  push_semaphore_->Signal();

  // Release the lock for the queue_mutex_
  pop_mutex_.unlock();

  // Return the popped element
  return memory_buffer;
}

/**
 * @brief Returns the maximum size of the memory buffer queues.
 *
 * @return Maximum size of the memory buffer queues.
 */
int pipeQueue::max_size() const { return max_size_; }

/**
 * @brief Returns the number of memory buffers in the input queue.
 *
 * @return Number of memory buffers in the input queue.
 */
int pipeQueue::queue_count() const { return queue_count_; }

/**
 * @brief Tries to get the ownership of the cpu resources to do any action
 */
void pipeQueue::wait_finish() { pop_semaphore_->Wait(); }
