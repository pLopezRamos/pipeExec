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

int SleeperMain(bool debug_flag, bool pu_debug_flag, bool profiling) {
  Sleeper sleeper_unit1; int unit1_threads = 1; 
  Sleeper sleeper_unit2; int unit2_threads = 9;
  Sleeper sleeper_unit3; int unit3_threads = 3;
  Sleeper sleeper_unit4; int unit4_threads = 1;
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


  if ( total_threads > number_of_data_items )
    allocated_memory = number_of_data_items + 1;
  else
    allocated_memory = total_threads + 1;

  printf("Running 4 stage pipe using queue size = %d\n", allocated_memory);
  MemoryManager *data_in = new MemoryManager(allocated_memory, debug_flag);
  for (int i = 0; i < allocated_memory; i += 1) {
//   pipeData *holder = new pipeData(new int(i));
    pipeData *holder = new pipeData(new int(0));
    holder->PushExtraData(new pipeData::DataKey{"profiling", &profiling});
    data_in->LoadMemoryManager(holder);
  }

/*  int* data1 = new int(1); 
  int* data2 = new int(9);
  int* data3 = new int(3);*/

  pipeData *data1 = new pipeData(new int(1));
  pipeData *data2 = new pipeData(new int(9));
  pipeData *data3 = new pipeData(new int(3));

  Pipeline *pipe = new Pipeline(&void_unit, data_in, 1, debug_flag, profiling);
  pipe->AddProcessingUnit(&sleeper_unit1, number_of_threads * unit1_threads, (void*)data1);
//  printf("\tStage 1 - sleep time %d, number of threads %d\n", *data1, number_of_threads * unit1_threads);
  pipe->AddProcessingUnit(&sleeper_unit2, number_of_threads * unit2_threads, (void*)data2);
  //printf("\tStage 2 - sleep time %d, number of threads %d\n", *data2, number_of_threads * unit2_threads);
  pipe->AddProcessingUnit(&sleeper_unit3, number_of_threads * unit3_threads, (void*)data3);
 // printf("\tStage 3 - sleep time %d, number of threads %d\n", *data3, number_of_threads * unit3_threads);
  pipe->AddProcessingUnit(&sleeper_unit4, number_of_threads * unit4_threads, (void*)data1);
//  printf("\tStage 4 - sleep time %d, number of threads %d\n", *data1, number_of_threads * unit4_threads);
  pipe->RunPipe();

  std::vector<int> sleeps = { 5, 8, 1, 3,7, 3, 3, 1, 1, 1, 1, 8,5,9, 2,3,1} ;

  printf("Processing %d data items\n", number_of_data_items);
  int *data_handler;
  int val;
  pipeData *data;
  for (int i = 0; i < number_of_data_items; ++i) {
    if (debug_flag) {
      printf("(main) Popping from IN\n");
    }
    data = (pipeData *)data_in->PopFromIn();
    data_handler = (int *)data->data();
//    val = distribution(gen);
//    val = sleeps[i % 17];
//    *data_handler = &val;
    printf("Sending item %d with a %ds sleep\n", i, *data_handler);
    if (debug_flag) {
      printf("(main) Pushing into OUT \n");
    }
    data_in->PushIntoOut((void*)data);
  }
  pipe->WaitFinish();

  pipe->Profile();

  return 0;
}

int main() {

  SleeperMain(false, false, false);

  exit(0);
}
/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
