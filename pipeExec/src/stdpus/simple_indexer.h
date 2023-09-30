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
 * @file simple_indexer.h
 *
 * @brief The header file for the SimpleIndexer processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef SIMPLE_INDEXER_H
#define SIMPLE_INDEXER_H

#include "pipeExec.h"

class SimpleIndexer : public ProcessingUnitInterface {
 public:
  SimpleIndexer();
  ~SimpleIndexer();

  void Init(void ** = nullptr) override;
  void Run(void *) override;
  void End() override;
  ProcessingUnitInterface *Clone() override;

 private:
  int32_t counter_;
};

#endif  // SIMPLE_INDEXER_H
