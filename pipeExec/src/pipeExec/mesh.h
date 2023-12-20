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
 * @file mesh.h
 *
 * @brief Declaration of the Mesh class methods
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
 * @class Mesh
 * @brief Class representing the pipeline for the processing of any type of
 * data.
 *
 * This class masks the processing of the data allowing the (main) thread to
 * only see one queue of data while inside the Mesh there are multiple
 * queues and threads consuming the data and processing it concurrently to speed
 * up the proccess.
 */
class Mesh
{
public:

  // Constructor for the Mesh class
  Mesh(ProcessingUnitInterface *, pipeQueue *, pipeQueue *, int, pipeData::dataPacket);

  // Destructor of the Mesh
  ~Mesh();

  // Adds a new processing unit to the Mesh
  PipeNode *meshAddProcessingUnit(ProcessingUnitInterface *, int, pipeData::dataPacket = nullptr, int = 2, int = 0, int = 0);

  PipeNode *meshInsertProcessingUnit(PipeNode *, ProcessingUnitInterface *, int, pipeData::dataPacket = nullptr, int = 2, int = 0, int = 0);

  // Runs the pipe making all the threads wait for an input
  int RunMesh();

  PipeNode *getHead() { return firstNode_; };
  PipeNode *getTail() { return lastNode_; };

  pipeMapper twoDimPipe;

private:
  std::vector<PipeNode *> execution_list_; /**< The list of nodes that need to
                                             be executed in order */
  std::mutex execution_mutex_;             /**< The mutex to safely run the nodes */
  std::mutex profiling_mutex_;             /**< The mutex to safely recollect times */
  int node_number_;                        /**< The number of nodes that are active */
  PipeNode *firstNode_;
  PipeNode *lastNode_;
  pipeMapper::nodeId prev_address_;
};
