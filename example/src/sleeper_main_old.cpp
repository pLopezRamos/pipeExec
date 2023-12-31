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
  int number_of_threads = 2;
  int total_threads = threads[0];
  pipeQueue* toPipe;
  for (int i = 1; i  < stages; ++i)
    total_threads += threads[i];
  total_threads *= number_of_threads;


  // Use a random device to seed the random number generator
  std::random_device rd;

  // Use the random device to generate a seed for the random number engine
  std::mt19937 gen(rd());

  // Define a distribution for the range of numbers you want
  std::uniform_int_distribution<int> distribution(5, 7); // Generates numbers between 1 and 10

  if (total_threads > number_of_data_items)
    allocated_memory = number_of_data_items + 1;
  else
    allocated_memory = total_threads + 1;

  allocated_memory = 40;
  allocated_memory *= stages; // the number of stages
  printf("Running %d stage pipe using queue size = %d\n", stages, allocated_memory);

  //MemoryManager *data_queue = new MemoryManager(allocated_memory, debug_flag);
  pipeQueue *dataIn = new pipeQueue(allocated_memory, debug_flag);
  pipeQueue *dataOut = new pipeQueue(allocated_memory, debug_flag);
  for (int i = 0; i < allocated_memory; ++i)
  {
    pipeData *holder = new pipeData(nullptr);
//    holder->PushExtraData(new pipeData::DataKey{sleeper_unit[0].getKey(), static_cast<void*>(new int(0))});
    holder->PushExtraData(new pipeData::DataKey{"profiling", &profiling});
//    data_queue->LoadMemoryManager(holder);
    dataIn->LoadQueue(holder);
  }

  Pipeline *pipe = new Pipeline(&void_unit, dataIn, dataOut, 1, debug_flag, profiling);
//  pipe->AddProcessingUnit(&drano_unit[0], 1 , static_cast<void*>(&adaptable));
  pipe->AddProcessingUnit(&sleeper_unit[0], number_of_threads * threads[0], static_cast<void *>(&sleepTime[0]), 15, 1);
    printf("\tStage %d - sleep time %d, number of threads %d\n", 0, sleepTime[0], number_of_threads * threads[0]);
  int di = 1;
  for (int i = 1; i < stages; ++i) {
    printf("\tStage %d - sleep time %d, number of threads %d\n", i, sleepTime[i], number_of_threads * threads[i]);
//    pipe->AddProcessingUnit(&drano_unit[di++], 1 , static_cast<void*>(&adaptable));
    toPipe = pipe->AddProcessingUnit(&sleeper_unit[i], number_of_threads * threads[i], static_cast<void *>(&sleepTime[i]), 15, 1);
  }
  pipe->RunPipe();

  std::vector<int> sleeps = {5, 8, 1, 3, 7, 3, 3, 1, 1, 1, 1, 8, 5, 9, 2, 3, 1};

  printf("Processing %d data items\n", number_of_data_items);

  int *val, *oldVal;
  pipeData *data;

  auto buff_count = allocated_memory;
  for (int i = 0; i < number_of_data_items; ++i)
  {
//    data = (pipeData *)data_queue->PopFromIn();
    data = (pipeData *)dataIn->PopFromQueue();
    --buff_count;
//    val = static_cast<int *>(data->GetExtraData(sleeper_unit[0].getKey()));
//    *val = distribution(gen);
    //    *val = sleeps[i % sleeps.size()];
//    std::cout << "Sending item " << i << " with a " << (int)*val << " sleep" << std::endl;
    std::cout << "Sending item " << i << std::endl;

//    data_queue->PushIntoOut((void *)data);
    toPipe->PushIntoQueue((void *)data);j
    ++buff_count;
  }

  std::cout << "Waiting for pipe to drain " << buff_count << " data buffers\n";

  for (int i = 0; i < buff_count; ++i) {
    std::cout << " i = " << i << std::endl;
    dataIn->PopFromQueue();
  }
//  pipe->WaitFinish();

  pipe->Profile();

  return 0;
}

int main()
{

  SleeperMain(false, false, false);

  exit(0);
}

