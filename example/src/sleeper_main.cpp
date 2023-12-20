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
#include <random>
#include <iostream>
#include <unistd.h> // for linux

int SleeperMain(bool debug_flag, bool pu_debug_flag, bool profiling)
{
  Sleeper sleepUnit[4];
  Drano dranoUnit[4];
  int thread[4] = {1, 1, 1, 1};
  int sleepT[4] = {1, 9, 3, 1};
  int stages = 4;
  bool adaptable = false;

//   std::cout << "In function " << __func__ << " line " << __LINE__ << std::endl;

  NullUnit void_unit;
  int number_of_data_items = 100;
  int threadMult = 1;
  int qSize = 10;

  printf("Running %d stage pipe using queue size = %d\n", stages, qSize);

  pipeQueue *dataIn = new pipeQueue(qSize, debug_flag);
  pipeQueue *dataOut = new pipeQueue(qSize, debug_flag);
  Pipeline *pipe = new Pipeline(new Sleeper, dataIn, dataOut, threadMult * thread[0], static_cast<pipeData::dataPacket>(&sleepT[0]), profiling);
  pipe->AddProcessingUnit(new Drano, 1, static_cast<pipeData::dataPacket>(&adaptable), 6);
  pipe->AddProcessingUnit(new Sleeper, threadMult * thread[1], static_cast<pipeData::dataPacket>(&sleepT[1]), 3, 20, 1);
  pipe->AddProcessingUnit(new Drano, 1, static_cast<pipeData::dataPacket>(&adaptable), 6);
  pipe->AddProcessingUnit(new Sleeper, threadMult * thread[2], static_cast<pipeData::dataPacket>(&sleepT[2]), 3, 20, 1);
  pipe->AddProcessingUnit(new Drano, 1, static_cast<pipeData::dataPacket>(&adaptable), 6);
  auto node = pipe->AddProcessingUnit(new Sleeper, threadMult * thread[3], static_cast<pipeData::dataPacket>(&sleepT[3]), 3, 10, 1);
  auto fromPipe = node->out_data_queue();
  pipe->RunPipe();

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
    }
    else
    {
      std::cout << "Got data from queue ";
    }

    // Do something with the data
//    sleep(1);

    dataId = *static_cast<int *>(data->GetExtraData("DATA_ID"));

    std::cout << "dataId =  " << dataId << std::endl;

    dataIn->Push(static_cast<pipeData::dataPacket>(data));
  }

  std::cout << "Waiting for pipe to drain " << allocated_memory << " data buffers\n";
  // sleep(5);
  for (int i = 0; i < allocated_memory; ++i)
  {
    std::cout << " Popping item = " << i;
    data = (pipeData *)fromPipe->Pop();
    dataId = *(int *)data->GetExtraData("_#DATA_ID#_");
    std::cout << " dataId =  " << dataId << std::endl;
  }

  pipe->Profile();

  return 0;
}

int main()
{

  SleeperMain(false, false, false);

  exit(0);
}
