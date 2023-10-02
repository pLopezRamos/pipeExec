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
 * @file double_pipe.h
 *
 * @brief The header file for the DoublePipe processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef DOUBLE_PIPE_H
#define DOUBLE_PIPE_H
#include "pipeExec.h"
/**
 * @class DoublePipe
 *
 * @brief This class instantiates a Pipe inside a pipe showing the potential of
 * LUCID at processing any data.
 */
class DoublePipe : public ProcessingUnitInterface {
 public:
  DoublePipe();
  ~DoublePipe();

  void Init(void* = nullptr);
  void Run(void*);
  void End();
  ProcessingUnitInterface* Clone();

 private:
  MemoryManager* sub_mem_manager;
  Pipeline* sub_pipeline;
};
#endif  // DOUBLE_PIPE_H
