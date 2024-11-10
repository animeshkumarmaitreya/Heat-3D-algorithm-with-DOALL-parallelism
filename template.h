#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>
#include <stdlib.h>
#include <numa.h>
#include <omp.h>

#define N 100  // Smaller grid for easier viewing
#define T 1000  // Number of time steps
#define ALPHA 0.01  // Diffusion coefficient
#define TILE_SIZE 4  // Tile size for cache optimization
// #define SUB_N 5  // Size of each sub-domain in i and j (assuming equal sizes for simplicity)
#define TEMP_SOURCE 1000.0 //Tempeeature of heat source

#endif