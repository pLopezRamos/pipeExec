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
  int threads[4] = { 1, 9, 3, 1};
  int sleepTime[4] = { 1, 9 , 3, 1 };
  int stages = 4;
  bool adaptable = true;

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

  //MemoryManager *data_queue = new MemoryManager(allocated_memory, debug_flag);
  pipeQueue *dataIn = new pipeQueue(allocated_memory, debug_flag);
  pipeQueue *dataOut = new pipeQueue(allocated_memory, debug_flag);
  Pipeline *pipe = new Pipeline(&void_unit, dataIn, dataOut, 1, debug_flag, profiling);
  pipeQueue* fromPipe;
  for (int i = 0; i < stages; ++i) {
    printf("\tStage %d - sleep time %d, number of threads %d\n", i, sleepTime[i], number_of_threads * threads[i]);
    fromPipe = pipe->AddProcessingUnit(&sleeper_unit[i], number_of_threads * threads[i], static_cast<void *>(&sleepTime[i]), 2);
  }
  pipe->RunPipe();

  printf("Processing %d data items\n", number_of_data_items);

  pipeData *data;
  for (int i = 0; i < allocated_memory; ++i)
  {
    data = new pipeData(nullptr);
    data->PushExtraData(new pipeData::DataKey{"profiling", &profiling});
    dataIn->PushIntoQueue(data);
    std::cout << "Sending item " << i << std::endl;
  }

  for (int i = 0; i < number_of_data_items - allocated_memory; ++i)
  {
    std::cout << "Popping item " << i << std::endl;
    data = (pipeData *)fromPipe->PopFromQueue();

    std::cout << "Pushing item " << i + allocated_memory << std::endl;

    dataIn->PushIntoQueue((void *)data);
  }

  std::cout << "Waiting for pipe to drain " << allocated_memory << " data buffers\n";

  for (int i = 0; i < allocated_memory; ++i) {
    std::cout << " Popping item = " << i << std::endl;
    fromPipe->PopFromQueue();
  }

  pipe->Profile();

  return 0;
}

int main()
{

  SleeperMain(false, false, false);

  exit(0);
}

