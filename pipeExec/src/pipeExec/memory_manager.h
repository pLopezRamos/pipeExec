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
 * @file memory_manager.h
 *
 * @brief The header file for the MemoryManager class
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "semaphore.h"

/**
 * @class MemoryManager
 *
 * @brief A memory manager that provides an interface for managing a fixed-size
 * queue of pointers to memory buffers.
 *
 * @details This class provides functionality to push and pop memory buffers in
 * and out of the queue, as well as a debug flag to print the status of the
 * MemoryManager. It also implements semaphores to ensure synchronization
 * between the producer and consumer threads.
 */
class MemoryManager {
 public:
  // Constructor for MemoryManager class
  // Throws MemoryManagerError::kBadSizing If the maximum size is less than 1
  MemoryManager(int, bool = false);

  // Destructor for MemoryManager.
  // Frees the buffers inside both queues and then frees the queues
  ~MemoryManager();

  // Pushes a memory buffer into the input queue.
  bool PushIntoIn(void *);

  // Pushes a memory buffer into the output queue.
  bool PushIntoOut(void *);

  // Pops a memory buffer from the input queue.
  // Throws MemoryManagerError::kNullPtr If the content to return is
  // null (it can't be processed)
  void *PopFromIn();

  // Pops a memory buffer from the output queue.
  // Throws MemoryManagerError::kNullPtr If the content to return is null (it
  // can't be processed)
  void *PopFromOut();

  // Loads a memory buffer into the queues.
  void LoadMemoryManager(void *);

  // Getter. Returns the number of memory buffers in the input queue.
  int in_queue_count() const;

  // Getter. Returns the number of memory buffers in the output queue.
  int out_queue_count() const;

  // Getter. Returns the maximum size of the memory buffer queues.
  int max_size() const;

  // Tries to get the ownership of the cpu resources to do any action
  void wait_finish();

  /**
   * @enum MemoryManagerError
   * @brief Enumerated type for possible errors in MemoryManager class
   *
   * This enumerated type defines the possible errors that can occur in the
   * MemoryManager class.
   */
  enum MemoryManagerError {
    kBadSizing,    /**< Requested size of the memory manager is less than 1.*/
    kNullPtr,      /**< Requested item in PopFromIn or PopFromOut is null */
    kEmptyQueue,   /**< Requestto PopFromIn or PopFromOut when empty */
  };

 private:
  void **in_queue_;  /**< Pointer to the input queue. */
  void **out_queue_; /**< Pointer to the output queue. */
  int max_size_;     /**< Maximum size of the memory buffer queues. */

  int rear_in_iterator_;   /**< Index of the rear of the input queue. */
  int front_in_iterator_;  /**< Index of the front of the input queue. */
  int rear_out_iterator_;  /**< Index of the rear of the output queue. */
  int front_out_iterator_; /**< Index of the front of the output queue. */

  std::atomic<int>
      in_queue_count_; /**< Number of memory buffers in the input queue. */
  std::atomic<int>
      out_queue_count_; /**< Number of memory buffers in the output queue. */

  Semaphore *in_semaphore_;  /**< Semaphore for the input queue. */
  Semaphore *out_semaphore_; /**< Semaphore for the output queue. */

  std::mutex push_in_mutex_;  /**< Mutex for pushing into the input queue. */
  std::mutex push_out_mutex_; /**< Mutex for pushing into the output queue. */
  std::mutex pop_in_mutex_;   /**< Mutex for popping from the input queue. */
  std::mutex pop_out_mutex_;  /**< Mutex for popping from the output queue. */

  bool debug_; /**< Boolean for showing the debug information*/
};

#endif
