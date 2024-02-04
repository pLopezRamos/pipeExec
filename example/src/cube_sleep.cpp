/*
 * LUCID (Unified Computing Image and Data-Processing) is a program to process
 * any type of data concurrently. Copyright (C) 2023 Lucas Hernández Abreu
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
 * Author:  Lucas Hernández Abreu
 * Contact: lucas.hernandez.09@ull.edu.es
 */

#include "sleeper.h"
#include "drano.h"
#include "null_unit.h"
#include "cube.h"
#include "distributerCube.h"
#include <random>
#include <iostream>
#include <unistd.h> // for linux

int CubeMain(bool debug_flag, bool pu_debug_flag, bool profiling)
{
  Drano dranoUnit[4];
  int thread[4] = {1, 1, 1, 1};
  int sleepT[4] = {1, 9, 3, 1};
  int stages = 4;
  bool adaptable = false;

  // std::cout << "In function " << __func__ << " line " << __LINE__ << std::endl;

  NullUnit void_unit;
  int number_of_data_items = 2500;
  unsigned int xRange = 5;
  unsigned int yRange = 5;
  unsigned int zRange = 3;
  unsigned int qSize = 10;

  std::cout << "In line " << __LINE__ << std::endl;

  auto *cube = new Cube(xRange, yRange, zRange, qSize, false);
  for (unsigned int x = 0; x < xRange; ++x)
  {
    for (unsigned int y = 0; y < yRange; ++y)
    {
      for (unsigned int z = 0; z < zRange; ++z)
      {
        auto node = cube->AddProcessingUnit(new Sleeper, 5, x, y, z, static_cast<pipeData::dataPacket>(&sleepT[0]),
                                            5, 20, 5);
      }
    }
  }

  std::vector<distributerCube::inputMesh> feeds;
  for (unsigned int x = 0; x < xRange; ++x) {
    for (unsigned int y = 0; y < yRange; ++y) {
      feeds.push_back({x, y});
    }
  }
  auto dataIn = new pipeQueue(qSize);
  auto dataOut = cube->out_queue();
  auto cube_start = new distributerCube(cube, dataIn, feeds);

  printf("Running %d stage pipe using queue size = %d\n", stages, qSize);

 cube->RunCube();

  printf("Processing %d data items\n", number_of_data_items);

  pipeData *data;
  int dataId;
  int allocated_memory = 0;
  for (int i = 0; i < number_of_data_items; ++i)
  {
    //  std::cout << __func__ << " : " << __LINE__ << std::endl;
    // Create new data items until they start returning from the pipe
   std::cout << __func__ << " : " << __LINE__ << std::endl;
    if ((data = (pipeData *)dataOut->Pop(false)) == nullptr)
    {
      std::cout << "i = " << i << " Allocate a new data item ";
      data = new pipeData(nullptr);
      data->PushExtraData(new pipeData::DataKey{"DATA_ID", static_cast<pipeData::dataPacket>(new int(i))});
      // data->PushExtraData(new pipeData::DataKey{sleeper_unit[0].getKey(), static_cast<void*>(new int(0))});
      ++allocated_memory;
      //      std::cout << __func__ << " : " << __LINE__ << std::endl;
    }
    else
    {
      std::cout << "i = " << i << " Got data from queue ";
    }

    // Do something with the data
    //    sleep(1);

    dataId = *static_cast<int *>(data->GetExtraData("DATA_ID"));

    std::cout << "dataId =  " << dataId << std::endl;

    cube_start->send(static_cast<pipeData::dataPacket>(data));
   std::cout << __func__ << " : " << __LINE__ << std::endl;
  }

  std::cout << "Waiting for pipe to drain " << allocated_memory << " data buffers\n";
  // sleep(5);
  for (int i = 0; i < allocated_memory; ++i)
  {
    std::cout << " Popping item = " << i;
    data = (pipeData *)dataOut->Pop();
    dataId = *(int *)data->GetExtraData("DATA_ID");
    std::cout << " dataId =  " << dataId << std::endl;
  }

  return 0;
}

int main()
{

  CubeMain(false, false, false);

  exit(0);
}
