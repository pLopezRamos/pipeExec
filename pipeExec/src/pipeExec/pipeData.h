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
 * @file pipeData.h
 *
 * @brief The header file for the pipeData class
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */

#ifndef PIPEDATA_H
#define PIPEDATA_H

//#include "memory_manager.h"
#include "pipeQueue.h"
//#include "pipe_node.h"

class PipeNode; // Forward defintion

/**
 * @class pipeData
 *
 * @brief A pipeData class that provides access to various forms of manipulation and
 * allocation for the processing units
 *
 * @details This class provides functionallity to store multiple data via keys
 * and a pointer to the original data that was stored initially with the
 * creation of the class.
 */
class pipeData {
  public:
    
    using dataPacket = void*;
    /**
     * @brief This structure works like a std::pair but its member names are
     * meaningful and are used to store data paired to a particular key.
     */
    struct DataKey {
      std::string key; /**< The key used to find the data associated */
      dataPacket data;      /**< The extra data stored*/
    };

    /**
     * @brief Set the DataKey structure to the given key and data
     *
     * @return True if the key is unique and flase otherwise.
     */
    bool setDataKey(std::string key, dataPacket data);

    /**
     * @brief Check if the given key exists in the pipeData object
     *
     * @return True if the key exists, false otgherwise
     */
    bool isKey(std::string key) const;

    // The data constructor
    pipeData(dataPacket, bool = false);

    // The data destructor
    ~pipeData();

    // Pushes the struct {key, data} to the extra data allocator
    void PushExtraData(DataKey *);

    // Gets the pointer to the extra data so you can manipulate it
    void *GetExtraData(std::string) const;

    // Gets the initial data stored in the class
dataPacket data() const;

    void *resetExtraData(std::string key, dataPacket newData);

    PipeNode *getNodeData() { return node; };

    void setNodeData(PipeNode *nodeData) { node = nodeData; };

  private:
    dataPacket data_;
    unsigned int index_;
    std::vector<DataKey *> extra_data_;
    bool debug_;
    PipeNode *node;
};

#endif
