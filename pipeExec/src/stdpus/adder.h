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
 * @file adder.h
 *
 * @brief The header file for the Adder processing unit.
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef ADDER_H
#define ADDER_H

#include "pipeExec.h"
#include <iostream>

/**
 * @class Adder
 *
 * @brief This class represents an adder, it gets a number and adds 1 to it
 */
class Adder : public ProcessingUnitInterface {
  public:

    Adder() { setKey("_#std#adder#_"); };

    void Init(void *data = nullptr) override;

    // Adds one to the integer pointer by the data pointer
    void Run(void *data) override;

    // Returns a new instance of Adder.
    ProcessingUnitInterface* Clone() override { return new Adder(); };

  private:

    int incValue_;
};

#endif
