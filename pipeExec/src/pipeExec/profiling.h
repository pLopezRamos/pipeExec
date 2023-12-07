/*
 * pipeExec is a library for creating concurrent proccesing pipes
 * 
 * Copyright (C) 2023 Lucas Hernández Abreu and Pablo López Ramos
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

#pragma once

#define TIME_POINT std::chrono::system_clock::time_point
#define STOPWATCH_NOW std::chrono::high_resolution_clock::now()
#define TIME_IN_MS(t1, t2) \
  std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()

/**
 * @desc This function returns the current cycles that has passed since the
 * last CPU reset.
 *
 * @return The current cycles that has passed since the last CPU reset
 */
inline uint64_t rdtsc() {
  uint64_t hi, lo;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (uint64_t)hi << 32 | lo;
}
