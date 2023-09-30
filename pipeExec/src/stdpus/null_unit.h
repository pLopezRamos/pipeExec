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
 * @file null_unit.h
 *
 * @brief The header file for the NullUnit processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef NULL_UNIT_H
#define NULL_UNIT_H

#include "pipeExec.h"

class NullUnit : public ProcessingUnitInterface {
 public:
  // Default constructor
  NullUnit();

  // Destructor
  ~NullUnit();

  // Does nothing
//  void Run(void*) override;
  void Run(void*);

  // Doesn't allocate nothing
//  void Init(void** = nullptr) override;
  void Init(void** = nullptr);

  // Doesn't delete nothing
//  void End();
  void End() override;

//  ProcessingUnitInterface* Clone() override;
  ProcessingUnitInterface* Clone();
};

#endif
