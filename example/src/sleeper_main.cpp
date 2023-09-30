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

int SleeperMain(bool debug_flag, bool pu_debug_flag, bool profiling) {
  Sleeper sleeper_unit1; int unit1_threads = 1;
  Sleeper sleeper_unit2; int unit2_threads = 2;
  Sleeper sleeper_unit3; int unit3_threads = 3;
  Sleeper sleeper_unit4; int unit4_threads = 1;
  NullUnit void_unit;
  int allocated_memory;
  // DATOS
  int number_of_data_items = 10000;
  // HILOS
  int number_of_threads = 100;
  int total_threads = (unit1_threads + unit2_threads + unit3_threads + unit4_threads) * number_of_threads;

  if ( total_threads > number_of_data_items )
    allocated_memory = number_of_data_items + 1;
  else
    allocated_memory = total_threads + 1;

  printf("Running 4 stage pipe using queue size = %d\n", allocated_memory);
  MemoryManager *data_in = new MemoryManager(allocated_memory, debug_flag);
  for (int i = 0; i < allocated_memory; i += 1) {
    pipeData *holder = new pipeData(new int(i));
    holder->PushExtraData(new pipeData::DataKey{"profiling", &profiling});
    data_in->LoadMemoryManager(holder);
  }

  Pipeline *pipe = new Pipeline(&void_unit, data_in, 1, debug_flag, profiling);
  pipe->AddProcessingUnit(&sleeper_unit1, number_of_threads * unit1_threads, "d", 1);
  printf("\tStage 1 - sleep time 1, number of threads %d\n", number_of_threads * unit1_threads);
  pipe->AddProcessingUnit(&sleeper_unit2, number_of_threads * unit2_threads, "d", 2);
  printf("\tStage 2 - sleep time 2, number of threads %d\n", number_of_threads * unit2_threads);
  pipe->AddProcessingUnit(&sleeper_unit3, number_of_threads * unit3_threads, "d", 3);
  printf("\tStage 3 - sleep time 3, number of threads %d\n", number_of_threads * unit3_threads);
  pipe->AddProcessingUnit(&sleeper_unit4, number_of_threads * unit4_threads, "d", 1);
  printf("\tStage 4 - sleep time 1, number of threads %d\n", number_of_threads * unit4_threads);
  pipe->RunPipe();

  printf("Processing %d data items\n", number_of_data_items);
  for (int i = 0; i < number_of_data_items; ++i) {
    if (debug_flag) {
      printf("(main) Popping from IN\n");
    }
    int *data_handler = (int *)data_in->PopFromIn();
    if (debug_flag) {
      printf("(main) Pushing into OUT \n");
    }
    data_in->PushIntoOut(data_handler);
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
