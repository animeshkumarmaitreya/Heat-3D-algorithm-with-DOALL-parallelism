#ifndef DOALL_H
#define DOALL_H

#include "template.h"

void parallel_heat3D(double*** grid, double*** new_grid) {
    for (int t = 0; t < T; t++) {
        #pragma omp parallel for collapse(3)
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