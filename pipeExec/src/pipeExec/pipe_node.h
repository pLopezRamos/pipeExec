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
 * @file pipe_node.h
 *
 * @brief Declaration of the PipeNode class methods
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */
#ifndef PIPE_NODE_H
#define PIPE_NODE_H

#include "processing_unit_interface.h"
#include <thread>
#include "profiling.h"

/**
 * @class PipeNode
 * @brief Class representing a single node in a processing pipeline
 *
 * This class represents a single node in a processing pipeline. It contains
 * a processing unit interface which is used to process the input data from
 * the previous node, and produce output data for the next node. It also
 * contains input and output memory managers for communication with the
 * previous and next nodes. Each node is executed in its own thread.
 */
class PipeNode {
 public:
  // Default constructor for PipeNode
  PipeNode();

  // Destructor for PipeNode
  ~PipeNode();

  // This method signals the end of the node's work, and ensures that all
  // threads have finished execution before returning.
  void EndNodeWork();

  // Gets the input memory manager of the current node
  MemoryManager *in_data_queue();

  // Gets the output memory manager of the current node
  MemoryManager *out_data_queue();

  // @brief Gets whether the current node is the last node in the pipeline
  bool last_node();

  // Gets the processing unit of the current node
  ProcessingUnitInterface *processing_unit();

  // Gets the number of instances of the current node
  int number_of_instances();

  // Gets the ID of the current node
  int node_id();

  // Gets a vector of running threads for the current node
  std::vector<std::thread *> &running_threads();

  // Gets the pointer to the extra_args
  void *extra_args();

  // Sets the input memory manager of the current node
  void in_data_queue(MemoryManager *);

  // Sets the output memory manager of the current node
  void out_data_queue(MemoryManager *);

  // Sets the boolean indicating if the node is the last in the
  // pipeline
  void last_node(bool);

  // Sets the processing unit of the node
  void processing_unit(ProcessingUnitInterface *);

  // Sets the number of instances of the current node
  void number_of_instances(int);

  // Sets the ID of the current node
  void node_id(int);

  // Pushes the thread to the list of running threads
  void PushThread(std::thread *);

  // Sets the extra_args
  void extra_args(void *);

 private:
  int node_id_;             /**< Id of the node */
  int number_of_instances_; /**< Number of instances of the processing unit */
  MemoryManager *in_data_queue_;  /**< Pointer to the input data queue */
  MemoryManager *out_data_queue_; /**< Pointer to the output data queue */
  ProcessingUnitInterface
      *processing_unit_; /**< Pointer to the ProcessingUnit to use */
  bool is_last_node_;    /**< True if it's the last node */
  std::vector<std::thread *>
      running_threads_; /**< The list with the running threads */
  void *extra_args_;
};

#endif
