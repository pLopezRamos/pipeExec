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
#include "null_unit.h"
#include <random>
#include <iostream>

int SleeperMain(bool debug_flag, bool pu_debug_flag, bool profiling)
{
  Sleeper sleeper_unit1, sleeper_unit2, sleeper_unit3, sleeper_unit4;
  int unit1_threads = 5, unit2_threads = 5, unit3_threads = 5, unit4_threads = 5;
  NullUnit void_unit;
  int allocated_memory;
  // DATOS
  int number_of_data_items = 1000;
  // HILOS
  int number_of_threads = 1;
  int total_threads = (unit1_threads + unit2_threads + unit3_threads + unit4_threads) * number_of_threads;

  // Use a random device to seed the random number generator
  std::random_device rd;

  // Use the random device to generate a seed for the random number engine
  std::mt19937 gen(rd());

  // Define a distribution for the range of numbers you want
  std::uniform_int_distribution<int> distribution(1, 10); // Generates numbers between 1 and 10

  if (total_threads > number_of_data_items)
    allocated_memory = number_of_data_items + 1;
  else
    allocated_memory = total_threads + 1;

  allocated_memory *= 4; // the number of stages
  printf("Running 4 stage pipe using queue size = %d\n", allocated_memory);
  MemoryManager *data_in = new MemoryManager(allocated_memory, debug_flag);
  for (int i = 0; i < allocated_memory; ++i)
  {
     pipeData *holder = new pipeData(new int(0));
    // pipeData *holder = new pipeData(nullptr);
    holder->PushExtraData(new pipeData::DataKey{sleeper_unit1.getKey(), static_cast<void*>(new int(0))});
    holder->PushExtraData(new pipeData::DataKey{"profiling", &profiling});
    data_in->LoadMemoryManager(holder);
  }

  auto d1 = new int(1);
  auto d2 = new int(9);
  auto d3 = new int(3);

  Pipeline *pipe = new Pipeline(&void_unit, data_in, 1, debug_flag, profiling);
  pipe->AddProcessingUnit(&sleeper_unit1, number_of_threads * unit1_threads, static_cast<void *>(d1));
  printf("\tStage 1 - sleep time %d, number of threads %d\n", *d1, number_of_threads * unit1_threads);
  pipe->AddProcessingUnit(&sleeper_unit2, number_of_threads * unit2_threads, static_cast<void *>(d2));
  printf("\tStage 2 - sleep time %d, number of threads %d\n", *d2, number_of_threads * unit2_threads);
  pipe->AddProcessingUnit(&sleeper_unit3, number_of_threads * unit3_threads, static_cast<void *>(d3));
  printf("\tStage 3 - sleep time %d, number of threads %d\n", *d3, number_of_threads * unit3_threads);
  pipe->AddProcessingUnit(&sleeper_unit4, number_of_threads * unit4_threads, static_cast<void *>(d1));
  printf("\tStage 4 - sleep time %d, number of threads %d\n", *d1, number_of_threads * unit4_threads);
  pipe->RunPipe();

  std::vector<int> sleeps = {5, 8, 1, 3, 7, 3, 3, 1, 1, 1, 1, 8, 5, 9, 2, 3, 1};

  printf("Processing %d data items\n", number_of_data_items);
  
  int *val, *oldVal;
  pipeData *data;

  for (int i = 0; i < number_of_data_items; ++i)
  {
    if (debug_flag)
    {
      printf("(main) Popping from IN\n");
    }
    data = (pipeData *)data_in->PopFromIn();
    //    val = distribution(gen);
    val = static_cast<int *>(data->GetExtraData(sleeper_unit1.getKey()));
    *val = sleeps[i % sleeps.size()];
    /*oldVal = static_cast<int *>(data->ResetExtraData((void *)val, sleeper_unit1.getKey())); // Used in case the element size changes
    if (oldVal != nullptr)
    {*/
      std::cout << "Sending item " << i << " with a " << (int)*val << " sleep" << std::endl;
    /*  delete oldVal;
    }
    else
    {
      std::cout << "oldVal = nullptr" << std::endl;
      std::cout << "Sending item" << i << " using init time for sleep" << std::endl;
    }*/
    if (debug_flag)
    {
      printf("(main) Pushing into OUT \n");
    }
    data_in->PushIntoOut((void *)data);
  }

  std::cout << "Waiting for pipe to drain ..." << std::endl;

  pipe->WaitFinish();

  pipe->Profile();

  return 0;
}

int main()
{

  SleeperMain(false, false, false);

  exit(0);
}

