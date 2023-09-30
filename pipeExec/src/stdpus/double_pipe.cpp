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
 * @file double_pipe.cc
 *
 * @brief The header file for the DoublePipe processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */
// TODO(lucashdez): COMMENT THIS FILE DOUBLEPIPE.CC

#ifndef DOUBLEPIPE_H
#define DOUBLEPIPE_H

#include "../pipeExec/processing_unit_interface.h"
#include "double_pipe.h"

DoublePipe::DoublePipe() {}
DoublePipe::~DoublePipe() {}

/**
 * @brief This method instantiates the pipeline and the MemoryManager
 * for the pipeline
 */
void DoublePipe::Init(void** pre_process_args) {
  // TODO(lucashdez): Instantiate pipeline
  // NOTE(lucashdez): Probably create a null processing unit
}

/**
 *
 *
 */
void DoublePipe::Run(void* data) {
  // TODO(lucashdez): Insert the processing units and run the pipe with the data
  MemoryManager* data_in = new MemoryManager(1, false);
  data_in->LoadMemoryManager(data);

  Pipeline* pipe = new Pipeline(new Adder(), data_in, 5, false);
  pipe->RunPipe();
  void* inside_data = data_in->PopFromIn();
  data_in->PushIntoOut(inside_data);

  pipe->WaitFinish();
}

/**
 *
 *
 */
void DoublePipe::End() {}

/**
 *
 *
 */
ProcessingUnitInterface* DoublePipe::Clone() { return new DoublePipe; }

#endif

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
