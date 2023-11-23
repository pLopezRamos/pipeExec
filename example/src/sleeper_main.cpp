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

int SleeperMain(bool debug_flag, bool pu_debug_flag, bool profiling)
{
  Sleeper sleeper_unit[4];
  Drano drano_unit[4];
  int threads[4] = { 1, 1, 1, 1};
  int sleepTime[4] = { 1, 9 , 3, 1 };
  int stages = 4;
  bool adaptable = false;

  NullUnit void_unit;
  int allocated_memory;
  int number_of_data_items = 100;
  int number_of_threads = 1;
  int total_threads = threads[0];
  for (int i = 1; i  < stages; ++i)
    total_threads += threads[i];
  total_threads *= number_of_threads;

  allocated_memory = 40;
  printf("Running %d stage pipe using queue size = %d\n", stages, allocated_memory);

  pipeQueue *dataIn = new pipeQueue(allocated_memory, debug_flag);
  pipeQueue *dataOut = new pipeQueue(allocated_memory, debug_flag);
  Pipeline *pipe = new Pipeline(&void_unit, dataIn, dataOut, 1, debug_flag, profiling);
  auto fromPipe = pipe->AddProcessingUnit(&sleeper_unit[0], number_of_threads * threads[0], static_cast<void *>(&sleepTime[0]), 2);
  int di = 0;
  for (int i = 1; i < stages; ++i) {
    pipe->AddProcessingUnit(&drano_unit[di++], 1 , static_cast<void*>(&adaptable));
    printf("\tStage %d - sleep time %d, number of threads %d\n", i, sleepTime[i], number_of_threads * threads[i]);
    fromPipe = pipe->AddProcessingUnit(&sleeper_unit[i], number_of_threads * threads[i], static_cast<void *>(&sleepTime[i]), 2);
  }
  pipe->RunPipe();

  printf("Processing %d data items\n", number_of_data_items);

  pipeData *data;
  for (int i = 0; i < allocated_memory; ++i)
  {
    data = new pipeData(nullptr);
   // data->PushExtraData(new pipeData::DataKey{sleeper_unit[0].getKey(), static_cast<void*>(new int(0))});
    dataIn->Push(data);
    std::cout << "Sending item " << i << std::endl;
  }

  for (int i = 0; i < number_of_data_items - allocated_memory; ++i)
  {
    data = (pipeData *)fromPipe->Pop();

    // Do something with the data

    std::cout << "Pushing item " << i + allocated_memory << std::endl;

    dataIn->Push((void *)data);
  }

  std::cout << "Waiting for pipe to drain " << allocated_memory << " data buffers\n";

  for (int i = 0; i < allocated_memory; ++i) {
    std::cout << " Popping item = " << i << std::endl;
    data = (pipeData *)fromPipe->Pop();
  }

  pipe->Profile();

  return 0;
}

int main()
{

  SleeperMain(false, false, false);

  exit(0);
}

