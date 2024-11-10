#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include "template.h"

// Wavefront parallelism for 3D heat conduction
void wavefront_heat3D(double*** grid, double*** new_grid) {
    for (int t = 0; t < T; t++) {
        // Process diagonal "wavefronts" in increasing order
        for (int wave = 2; wave < 2 * (N - 1); wave++) {
            #pragma omp parallel for collapse(2)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    int k = wave - i - j;
                    if (k >= 1 && k < N - 1) {
                        new_grid[i][j][k] = grid[i][j][k] +
                            ALPHA * (grid[i - 1][j][k] + grid[i + 1][j][k] +
                                     grid[i][j - 1][k] + grid[i][j + 1][k] +
                                     grid[i][j][k - 1] + grid[i][j][k + 1] -
                                     6 * grid[i][j][k]);
                    }
                }
            }
            // Wait for all threads to finish processing the current wavefront before moving to the next
            #pragma omp barrier
        }
        // Swap pointers instead of grids
        double*** temp = grid;
        grid = new_grid;
        new_grid = temp;
    }
}

#endif