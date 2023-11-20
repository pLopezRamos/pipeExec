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
 * @file processing_unit_interface.h
 *
 * @brief The header file for the virtual ProcessingUnitInterface class
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef PROCESSING_UNIT_INTERFACE_H
#define PROCESSING_UNIT_INTERFACE_H

#include "pipeData.h"
/**
 * @class ProcessingUnitInterface
 *
 * @brief This class is the parent class of all the processing units we will
 * create
 */
class ProcessingUnitInterface
{
public:
  /**
   * @brief Use this function to allocate memory for the variables that need
   * it and initialize some of them
   */
  virtual void Init(void ** = nullptr) { return; };
  virtual void Init(void * = nullptr) { return; };

  /**
   * @brief Use this function to process the data. It's recommended to recast
   * the void* into a Data* to use all its methods. (ONLY IF THE DATA IS A
   * DATA* ALLOCATION)
   *
   * @param data The data to manipulate
   */
  virtual void Run(void *data) = 0;

  /**
   * @brief Use this function to free all the memory allocated in the Start
   * method
   */
  //  virtual void End() = 0;
  virtual void End(void *data = nullptr) { return; };

  /**
   * @brief Returns new pointer to a processing unit class. Use {return new
   * [class-name]} || Use nullptr to disallow clonning
   * @return Pointer to the new derived class || nullptr to disallow
   * cloning
   */
  virtual ProcessingUnitInterface *Clone() { return nullptr; };

  std::string getKey() { return extraDataKey; };

  void setKey(std::string key = "_#STD#NO#KEY#_") { extraDataKey = key; };

  /// @brief Returns the basic data from the pipeData object
  /// @param data - A pointer to the pipeData object
  /// @return The void pointer from the pipeData object
  void *getData(void *data)
  {
    pipeData* pipeD = static_cast<pipeData*>(data);
    return (pipeD != nullptr) ? pipeD->data() : nullptr;
  }

  /// @brief Get the data pointer assiciated with the given key
  /// @param data a pointer to the pipeData obejct
  /// @param key The key to search for
  /// @return The data associated to the key or nullptr if not found
  void *getExtraData(void *data, const std::string key)
  {
    if (data)
    {
      auto hold = static_cast<pipeData *>(data);
      return hold->GetExtraData(key);
    }
    else
    {
      return nullptr;
    }
  }

protected:

  std::string extraDataKey;

};
#endif
