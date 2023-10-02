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
 * @brief This class makes the thread sleep for x seconds, given in the start
 * arguments
 */
class Sleeper : public ProcessingUnitInterface {
 public:
  // Default constructor
  Sleeper();

  // Default destructor
  ~Sleeper();

  // This method receives the arguments for the sleeping time
  void Init(void * = nullptr);

  // This method sets the thread to sleep for x seconds
  void Run(void *);

  // Does nothing
  void End();

  // This function clones the instance of the sleeper procesing unit
  ProcessingUnitInterface *Clone();

 private:
  uint32_t seconds_to_sleep_;
};

#endif  // SLEEPER_H
