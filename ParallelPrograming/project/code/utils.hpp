#pragma once

#include <ctime>
#include <omp.h>

double start_t, end_t;

void start_time() { start_t = omp_get_wtime(); }
void end_time() { end_t = omp_get_wtime(); }

double get_elapsed_time() { return end_t - start_t; }
