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
 * Contact: lopez.ramos.pblo@gmail.com
 */

/**
 * @file sleeper.h
 *
 * @brief The header file for the Drano processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef DRANO_H
#define DRANO_H

#include "pipeExec.h"
#include "pipeData.h" // Include the necessary header
#include <iostream>
#include <chrono>
#include <thread>

/**
 * @class Drano
 *
 * @brief This class makes the thread sleep for x seconds, given in the start
 *
 * arguments
 */
class Drano : public ProcessingUnitInterface {
 public:
  // Default constructor
  Drano();

  // Default destructor
  ~Drano();

  /**
   * @brief initialize
   *
   * @details If an argument is give, it will set the minimun time in miliseconds
   * to consider that the out queue is full
   *
   * @param mSeconds - a pointer to an unsigned integer. If null 100 ms is et.
   *
   * @return void
   */
  void Init(void *mSeconds = nullptr) override;

  /**
   * @brief Execute the sleep
   *
   * @details If the sleep time given is less or equal to 0, then use the value
   * set in INit(), otherwise use the value it point to.
   *
   * @param sleepTime - a pointer to an integer
   *
   * @return void
   */
  void Run(void *data) override;

private:

  unsigned int lastCount_;
  unsigned int ignoreCount_;
  bool adaptable_;
};

#endif  // DRANO_H
