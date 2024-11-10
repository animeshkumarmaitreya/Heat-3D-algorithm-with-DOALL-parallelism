#ifndef NUMA_H
#define NUMA_H

#include "template.h"

// Function to allocate 3D grid dynamically with NUMA awareness
double*** allocate_grid_numa(int node) {
    double*** grid = (double***)numa_alloc_onnode(N * sizeof(double**), node);
    for (int i = 0; i < N; i++) {
        grid[i] = (double**)numa_alloc_onnode(N * sizeof(double*), node);
        for (int j = 0; j < N; j++) {
            grid[i][j] = (double*)numa_alloc_onnode(N * sizeof(double), node);
        }
    }
    return grid;
}

// Function to free dynamically allocated NUMA grid
void free_grid_numa(double*** grid) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            numa_free(grid[i][j], N * sizeof(double));
        }
        numa_free(grid[i], N * sizeof(double*));
    }
    numa_free(grid, N * sizeof(double**));
}

// Sequential or DOALL heat conduction with NUMA-aware allocation
void numa_heat3D(double*** grid, double*** new_grid) {
    for (int t = 0; t < T; t++) {
        // #pragma omp parallel for collapse(3) // not now as sequential
        for (int i = 1; i < N-1; i++) {
            for (int j = 1; j < N-1; j++) {
                for (int k = 1; k < N-1; k++) {
                    new_grid[i][j][k] = grid[i][j][k] +
                        ALPHA * (grid[i-1][j][k] + grid[i+1][j][k] + grid[i][j-1][k] + 
                                 grid[i][j+1][k] + grid[i][j][k-1] + grid[i][j][k+1] -
                                 6 * grid[i][j][k]);
                }
            }
        }
        // Swap pointers instead of grids
        double*** temp = grid;
        grid = new_grid;
        new_grid = temp;
    }
}

#endif