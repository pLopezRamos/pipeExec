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
 * Author:  Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

/**
 * @file memory_manager.cc
 *
 * @brief The source file for the MemoryManager class
 * @details Here all the methods of the class are defined along with the
 * security and error handling for the class
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 *
 * Contact: lopez.ramos.pablo@gmail.com
 */

#include "memory_manager.h"
#include <malloc.h>
#include <cstdio>

/**
 * @brief Constructor for MemoryManager class
 *
 * @param mx_size The maximum number of memory buffers the queue can hold
 * @param debug The flag for the debug information
 * @throw invalid_argument If the maximum size is less than 1
 */
MemoryManager::MemoryManager(int mx_size, bool debug)
  : max_size_(mx_size), debug_(debug), rear_in_iterator_(-1), rear_out_iterator_(-1),
  front_in_iterator_(0), front_out_iterator_(0), out_queue_count_(0), in_queue_count_(0) {
    // Validate the maximum size parameter
    if (mx_size < 1) {
      throw std::invalid_argument("mx_size has to be grater 0");
    }

    // Allocate memory for in_queue_ and out_queue_ using malloc
    in_queue_ = (void**)malloc(max_size_ * sizeof(void*));
    out_queue_ = (void**)malloc(max_size_ * sizeof(void*));

    // Initialize all elements of in_queue_ to nullptr using a loop
    for (int it = 0; it < max_size_; ++it) {
      in_queue_[it] = nullptr;
    }

    // These semaphores are initialized with counts set to 0
    // and a specified type (kIn or kOut) for synchronization
    in_semaphore_ = new Semaphore(0);
    out_semaphore_ = new Semaphore(0);
  }

/**
 * @brief Destructor for MemoryManager.
 * @details Frees the buffers inside both queues and then frees the queues
 */
MemoryManager::~MemoryManager() {
  // Free the buffers inside the in_queue_ and out_queue_ arrays
  for (int it = 0; it < max_size_; ++it) {
    free(in_queue_[it]);
    free(out_queue_[it]);
  }

  // Free both arrays
  free(in_queue_);
  free(out_queue_);
}

/**
 * @brief Pushes a memory buffer into the input queue.
 *
 * @param data Pointer to the memory buffer.
 *
 * @return True if the input queue is not full, false otherwise.
 */
 bool MemoryManager::PushIntoIn(void *data) {
  // Acquire the lock for the in_queue_mutex_
  // This ensures that only one thread can access the in_queue_ array at a time
  push_in_mutex_.lock();

  // Check if the queue is full
  if (in_queue_count_ == max_size_) {
    // The queue is full, so we cannot push the data into the queue
    push_in_mutex_.unlock();
    return false;
  }

  // Push the data into the queue
  rear_in_iterator_ += 1;
  in_queue_[rear_in_iterator_] = data;

  // If the rear iterator has reached the end of the array, wrap it around to the beginning
  if (rear_in_iterator_ == max_size_ - 1) {
    rear_in_iterator_ = -1;
  }

  // Increment the in_queue_count_
  in_queue_count_ += 1;

  // Release the lock for the in_queue_mutex_
  push_in_mutex_.unlock();

  // Signal the in_semaphore_ semaphore to wake up a thread that is waiting to pop an element from the in_queue_
  in_semaphore_->Signal();

  // Return true to indicate that the data was successfully pushed into the queue
  return true;
}

/**
 * @brief Pushes a memory buffer into the output queue.
 *
 * @param data Pointer to the memory buffer.
 *
 * @return True if the output queue is not full, false otherwise.
 */
 bool MemoryManager::PushIntoOut(void *data) {
  // Acquire the lock for the out_queue_mutex_
  // This ensures that only one thread can access the out_queue_ array at a time
  push_out_mutex_.lock();

  // Check if the queue is full
  if (out_queue_count_ == max_size_) {
    // The queue is full, so we cannot push the data into the queue
    push_out_mutex_.unlock();
    return false;
  }

  // Push the data into the queue
  rear_out_iterator_ += 1;
  out_queue_[rear_out_iterator_] = data;

  // If the rear iterator has reached the end of the array, wrap it around to the beginning
  if (rear_out_iterator_ == max_size_ - 1) {
    rear_out_iterator_ = -1;
  }

  // Increment the out_queue_count_
  out_queue_count_ += 1;

  // Release the lock for the out_queue_mutex_
  push_out_mutex_.unlock();

  // Signal the out_semaphore_ semaphore to wake up a thread that is waiting to pop an element from the out_queue_
  out_semaphore_->Signal();

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
void *MemoryManager::PopFromIn() {
  // Acquire the lock for the in_queue_mutex_
  // This ensures that only one thread can access the in_queue_ array at a time
  pop_in_mutex_.lock();

  // Wait for the in_semaphore_ semaphore to be signaled, indicating that there is an element in the in_queue_
  in_semaphore_->Wait();

  // Check if the queue is empty
  if (in_queue_count_ == 0) {
    // The queue is empty, so we cannot pop an element from the queue
    pop_in_mutex_.unlock();
    throw std::out_of_range("The in queue is empty, nothing to return.");
  }

  // Pop the element from the queue
  void *memory_buffer = in_queue_[front_in_iterator_];
  in_queue_[front_in_iterator_] = nullptr;

  // Increment the front iterator
  front_in_iterator_ = (front_in_iterator_ + 1) % max_size_;

  // Decrement the in_queue_count_
  in_queue_count_ -= 1;

  // Release the lock for the in_queue_mutex_
  pop_in_mutex_.unlock();

  // Return the popped element
  return memory_buffer;
}

/**
 * @brief Pops a memory buffer from the output queue.
 *
 * @throw out_of_range If the content to return is null (it
 * can't be processed)
 *
 * @return Pointer to the memory buffer.
 */
void *MemoryManager::PopFromOut() {
  // Acquire the lock for the out_queue_mutex_
  // This ensures that only one thread can access the out_queue_ array at a time
  pop_out_mutex_.lock();

  // Wait for the out_semaphore_ semaphore to be signaled, indicating that there is an element in the out_queue_
  out_semaphore_->Wait();

  // Check if the queue is empty
  if (out_queue_count_ == 0) {
    // The queue is empty, so we cannot pop an element from the queue
    pop_out_mutex_.unlock();
    throw std::out_of_range("The out queue is empty, nothing to return.");
  }

  // Pop the element from the queue
  void *memory_buffer = out_queue_[front_out_iterator_];
  out_queue_[front_out_iterator_] = nullptr;

  // Increment the front iterator
  front_out_iterator_ = (front_out_iterator_ + 1) % max_size_;

  // Decrement the out_queue_count_
  out_queue_count_ -= 1;

  // Release the lock for the out_queue_mutex_
  pop_out_mutex_.unlock();

  // Return the popped element
  return memory_buffer;
}

/**
 * @brief Returns the maximum size of the memory buffer queues.
 *
 * @return Maximum size of the memory buffer queues.
 */
int MemoryManager::max_size() const { return max_size_; }

/**
 * @brief Loads a memory buffer into the queues.
 *
 * @param data Pointer to the memory buffer.
 */
void MemoryManager::LoadMemoryManager(void *data) {
//  PushIntoOut(data);          // Should be just a PushIntoIn(data) CHECK
//  PushIntoIn(PopFromOut());
  PushIntoIn(data);
}

/**
 * @brief Returns the number of memory buffers in the input queue.
 *
 * @return Number of memory buffers in the input queue.
 */
int MemoryManager::in_queue_count() const { return in_queue_count_; }

/**
 * @brief Returns the number of memory buffers in the output queue.
 *
 * @return Number of memory buffers in the output queue.
 */
int MemoryManager::out_queue_count() const { return out_queue_count_; }

/**
 * @brief Tries to get the ownership of the cpu resources to do any action
 */
void MemoryManager::wait_finish() { in_semaphore_->Wait(); }

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
