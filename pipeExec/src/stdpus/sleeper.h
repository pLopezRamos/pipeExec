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
 * @brief The header file for the Sleeper processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef SLEEPER_H
#define SLEEPER_H

#include "pipeExec.h"
#include <thread>

/**
 * @class Sleeper
 *
 * @brief This class makes the thread sleep for x seconds, given in the start
 *
 * arguments
 */
class Sleeper : public ProcessingUnitInterface {
 public:
  // Default constructor
  Sleeper();

  // Default destructor
  ~Sleeper();

  /**
   * @brief initialize
   *
   * @details If an argument is give, it will set the sleep time to what
   * it points. If the pointer is null then set the time to 0.
   *
   * @param initData - a pointer to an integer
   *
   * @return void
   */
  void Init(void *initData = nullptr) override;

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
  void Run(void *sleepTime) override;

  // This function clones the instance of the sleeper procesing unit
  ProcessingUnitInterface *Clone() override { return new Sleeper(); };

 private:
  int seconds_to_sleep_;
};

#endif  // SLEEPER_H
