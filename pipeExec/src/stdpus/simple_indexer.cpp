/*
 * pipeExec is a library for creating concurrent proccesing pipes
 *
 * Copyright (C) 2023 Lucas Hernández Abreu and Pablo López Ramos
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author:  Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

/**
 * @file simple_indexer.cc
 *
 * @brief The definition file for the SimpleIndexer processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#include "simple_indexer.h"

/**
 * @brief Default constructor of the class
 */
SimpleIndexer::SimpleIndexer() {}

/**
 * @brief Default destructor of the class
 */
SimpleIndexer::~SimpleIndexer() {}

/**
 * @brief Sets the internal counter to 0
 */
void SimpleIndexer::Init(void** pre_process_args) { counter_ = 0; }

/**
 * @brief Gets the pipeData from the pipe and indexes it by adding the extra data
 * "part_index"
 * @param data The data to be manipulated
 */
void SimpleIndexer::Run(void* data) {
  pipeData* handler = (pipeData*)data;

  handler->PushExtraData(new pipeData::DataKey({"part_index", new int(counter_)}));
  counter_++;
}

/**
 * @brief Use this function to delete any allocated memory inside the Start or
 * Run methods.
 */
void SimpleIndexer::End() {}

/**
 * @brief Returns a pointer to a new instance of the class.
 * @return The new instance of the class
 */
ProcessingUnitInterface* SimpleIndexer::Clone() { return new SimpleIndexer; }

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
