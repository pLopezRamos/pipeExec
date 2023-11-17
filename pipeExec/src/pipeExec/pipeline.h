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
 * @file pipeline.h
 *
 * @brief Declaration of the Pipeline class methods
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */
#ifndef PIPELINE_H
#define PIPELINE_H

#include "pipe_node.h"
#include <algorithm>
#include <stdarg.h>

/**
 * @class Pipeline
 * @brief Class representing the pipeline for the processing of any type of
 * data.
 *
 * This class masks the processing of the data allowing the (main) thread to
 * only see one queue of data while inside the Pipeline there are multiple
 * queues and threads consuming the data and processing it concurrently to speed
 * up the proccess.
 */
class Pipeline {
  public:
    enum PipelineError {
      kBadArgumentFormat,
      kBadArgumentType,
    };

    enum nodeCmd { NO_OP, ADD_THR, END_THR };
    /**
     * @brief This struct holds the information needed for the profiling of the
     * processing unit
     * @desc This struct holds all the information that the pipe will print when
     * the print_profile method function is called
     */
    struct Profiling {
      int32_t node_id;   /**< The id of the node to profile */
      int32_t thread_id; /**< The id of the thread that executed the processing unit
                          */
      uint64_t cycles_start; /**< The timestamp from the tsc in the CPU at the
                               start of the RunNode function */
      uint64_t cycles_end;   /**< The timestamp from the tsc in the CPU at the end
                               of the RunNode function */
      TIME_POINT time_start; /**< The clock time at the start of the RunNode function */
      TIME_POINT time_end; /**< The clock time at the end of the RunNode function */
      int64_t sys_time_start; /**< The system time at the start of the RunNode
                                function */
      int64_t sys_time_end; /**< The system time at the end of the RunNode function */
    };
    // Constructor for the Pipeline class
    Pipeline(ProcessingUnitInterface *, MemoryManager *, int, bool = false,
        bool = false);

    // Destructor of the Pipeline
    ~Pipeline();

    // Adds a new processing unit to the Pipeline
    void AddProcessingUnit(ProcessingUnitInterface *, int, void * = nullptr, int = 0, int = 0);

    // Runs the pipe making all the threads wait for an input
    int RunPipe();

    // Waits until all the threads finished processing
    void WaitFinish();

    void Profile();

  private:
    std::vector<PipeNode *> execution_list_; /**< The list of nodes that need to
                                               be executed in order */
    std::mutex execution_mutex_; /**< The mutex to safely run the nodes */
    std::mutex profiling_mutex_; /**< The mutex to safely recollect times */
    int node_number_;            /**< The number of nodes that are active */
    bool debug_;                 /**< The flag to show debug information */
    bool show_profiling_;        /**< The flag to show profiling information */
    std::vector<Profiling> profiling_list_; /**< The list of profiling information */
};

#endif
