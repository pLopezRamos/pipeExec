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
#include "mesh.h"
#include "distributer.h"
#include <random>
#include <iostream>
#include <unistd.h> // for linux

int MeshMain(bool debug_flag, bool pu_debug_flag, bool profiling)
{
  Drano dranoUnit[4];
  int thread[4] = {1, 1, 1, 1};
  int sleepT[4] = {1, 9, 3, 1};
  int stages = 4;
  bool adaptable = false;

//std::cout << "In function " << __func__ << " line " << __LINE__ << std::endl;

  NullUnit void_unit;
  int number_of_data_items = 1000;
  unsigned int xRange = 10;
  unsigned int yRange = 10;
  unsigned int qSize = 5;

  Mesh *mesh = new Mesh(xRange, yRange, 5, false);
  for (unsigned int x = 0; x < xRange; ++x)
  {
    for (unsigned int y = 0; y < yRange; ++y)
    {
      auto node = mesh->AddProcessingUnit(new Sleeper, 3, x, y, static_cast<pipeData::dataPacket>(&sleepT[2]),
                                          5, 20, 5);
    }
  }

  std::vector<unsigned int> feeds = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto dataIn = new pipeQueue(qSize);
  auto dataOut = mesh->out_queue();
  auto mesh_start = new distributer(mesh, dataIn, feeds);

  printf("Running %d stage pipe using queue size = %d\n", stages, qSize);

  mesh->RunMesh();

  printf("Processing %d data items\n", number_of_data_items);

  pipeData *data;
  int dataId;
  int allocated_memory = 0;
  for (int i = 0; i < number_of_data_items; ++i)
  {
    //  std::cout << __func__ << " : " << __LINE__ << std::endl;
    // Create new data items until they start returning from the pipe
    if ((data = (pipeData *)dataOut->Pop(false)) == nullptr)
    {
      std::cout << "Allocate a new data item ";
      data = new pipeData(nullptr);
      data->PushExtraData(new pipeData::DataKey{"DATA_ID", static_cast<pipeData::dataPacket>(new int(i))});
      // data->PushExtraData(new pipeData::DataKey{sleeper_unit[0].getKey(), static_cast<void*>(new int(0))});
      ++allocated_memory;
//      std::cout << __func__ << " : " << __LINE__ << std::endl;
    }
    else
    {
      std::cout << "Got data from queue ";
    }

    // Do something with the data
    //    sleep(1);

    dataId = *static_cast<int *>(data->GetExtraData("DATA_ID"));

    std::cout << "dataId =  " << dataId << std::endl;

    mesh_start->send(static_cast<pipeData::dataPacket>(data));
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

  MeshMain(false, false, false);

  exit(0);
}
