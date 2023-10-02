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
 * @file indexer.h
 *
 * @brief The header file for the Indexer processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#include "pipeExec.h"

#ifndef INDEXER_H
#define INDEXER_H

/**
 * @class Indexer
 * @description This class represents an Indexer, it gets data and indexes it if
 * it is broken into parts to reconstruct it correctly later. NOTES: DONT
 * CLONE IT
 */
class Indexer : public ProcessingUnitInterface {
 public:
  Indexer();
  ~Indexer();

  void Init(void * = nullptr) override;
  void Run(void *) override;
  void End() override;
  ProcessingUnitInterface *Clone() override;

 private:
  struct TableIndexer {
    int64_t id;
    int64_t count;
  };

  TableIndexer *FindTableEntry(int);

  TableIndexer *lookup_table_;
  int32_t table_size_;
};
#endif
