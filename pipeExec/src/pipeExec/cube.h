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
 * @file cube.h
 *
 * @brief Declaration of the Cube class methods
 *
 * @author Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */
#pragma once

#include "pipe_node.h"
#include "pipeData.h"
#include "pipeMapper.h"
#include <algorithm>
#include <stdarg.h>

/**
 * @class Cube
 * @brief Class representing the pipeline for the processing of any type of
 * data.
 *
 * This class masks the processing of the data allowing the (main) thread to
 * only see one queue of data while inside the Cube there are multiple
 * queues and threads consuming the data and processing it concurrently to speed
 * up the proccess.
 */
class Cube
{
public:
  // Constructor for the Cube class
  Cube(unsigned int xRange, unsigned int yRange, unsigned int zRange, unsigned int queueSize, bool closed = false);

  // Destructor of the Cube
  ~Cube();

  // Adds a new processing unit to the Cube
  PipeNode *AddProcessingUnit(ProcessingUnitInterface *procUnit, int instances, unsigned int x, unsigned int y, unsigned int z, pipeData::dataPacket initData = nullptr,
                              int maxInstances = 0, int minInstances = 0, unsigned int queueSize = 0);

  // Runs the pipe making all the threads wait for an input
  int RunCube();

  PipeNode *getHead() { return firstNode_; };
  PipeNode *getTail() { return lastNode_; };

  void out_queue(pipeQueue *queue) { out_queue_ = queue; };
  pipeQueue *out_queue() { return out_queue_; };

  pipeMapper *threeDimPipe;

private:
  std::vector<PipeNode *> execution_list_; /**< The list of nodes that need to
                                             be executed in order */
  std::mutex execution_mutex_;             /**< The mutex to safely run the nodes */
  std::mutex profiling_mutex_;             /**< The mutex to safely recollect times */
  int node_number_;                        /**< The number of nodes that are active */
  PipeNode *firstNode_;
  PipeNode *lastNode_;
  pipeMapper::nodeId prev_address_;
  unsigned int xRange_;
  unsigned int yRange_;
  unsigned int zRange_;
  pipeQueue *out_queue_;
};
