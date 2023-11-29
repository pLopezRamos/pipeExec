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
 * @file pipeQueue.h
 *
 * @brief The header file for the pipeQueue class
 *
 * @author Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef PIPEQUEUE_H
#define PIPEQUEUE_H

#include "semaphore.h"

/**
 * @class pipeQueue
 *
 * @brief A queue interface for managing a fixed-size queue of pointers to memory buffers.
 *
 * @details This class provides functionality to push and pop memory buffers in
 * and out of the queue. It also implements semaphores to ensure synchronization
 * between the producer and consumer threads.
 */
class pipeQueue {
 public:
  // Constructor for pipeQueue class
  // Throws pipeQueueError::kBadSizing If the maximum size is less than 1
  pipeQueue(int, bool = false);

  // Destructor for pipeQueue.
  // Frees the buffers inside both queues and then frees the queues
  ~pipeQueue();

  // Pushes a memory buffer into the input queue.
  bool Push(void *);

  // Pops a memory buffer from the input queue.
  // Throws pipeQueueError::kNullPtr If the content to return is
  // null (it can't be processed)
  void *Pop(bool = true);

  // Loads a memory buffer into the queues.
  void LoadpipeQueue(void *);

  // Getter. Returns the number of memory buffers in the input queue.
  int queue_count() const;

  // Getter. Returns the maximum size of the memory buffer queues.
  int max_size() const;

  // Wait until the queue is full again
  void wait_finish();

  /**
   * @enum pipeQueueError
   * @brief Enumerated type for possible errors in pipeQueue class
   *
   * This enumerated type defines the possible errors that can occur in the
   * pipeQueue class.
   */
  enum pipeQueueError {
    kBadSizing,    /**< Requested size of the memory manager is less than 1.*/
    kNullPtr,      /**< Requested item in PopFromIn or PopFromOut is null */
    kEmptyQueue,   /**< Requestto PopFromIn or PopFromOut when empty */
  };

 private:
  void **queue_;  /**< Pointer to the input queue. */
  int max_size_;     /**< Maximum size of the memory buffer queues. */

  int rear_iterator_;   /**< Index of the rear of the input queue. */
  int front_iterator_;  /**< Index of the front of the input queue. */

  std::atomic<int>
      queue_count_; /**< Number of memory buffers in the input queue. */

  Semaphore *pop_semaphore_;  /**< Semaphore for the input queue. */
  Semaphore *push_semaphore_;  /**< Semaphore for the input queue. */

  std::mutex push_mutex_;  /**< Mutex for pushing into the input queue. */
  std::mutex pop_mutex_;   /**< Mutex for popping from the input queue. */

  bool debug_; /**< Boolean for showing the debug information*/
};

#endif
