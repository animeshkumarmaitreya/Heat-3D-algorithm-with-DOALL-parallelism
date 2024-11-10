#ifndef TILING_H
#define TILING_H

#include "template.h"

void tiled_heat3D(double*** grid, double*** new_grid) {
    for (int t = 0; t < T; t++) {
        #pragma omp parallel for collapse(3)
        for (int i_tile = 1; i_tile < N-1; i_tile += TILE_SIZE) {
            for (int j_tile = 1; j_tile < N-1; j_tile += TILE_SIZE) {
                for (int k_tile = 1; k_tile < N-1; k_tile += TILE_SIZE) {
                    // Process each tile
                    for (int i = i_tile; i < i_tile + TILE_SIZE && i < N-1; i++) {
                        for (int j = j_tile; j < j_tile + TILE_SIZE && j < N-1; j++) {
                            for (int k = k_tile; k < k_tile + TILE_SIZE && k < N-1; k++) {
                                new_grid[i][j][k] = grid[i][j][k] +
                                    ALPHA * (grid[i-1][j][k] + grid[i+1][j][k] + 
                                             grid[i][j-1][k] + grid[i][j+1][k] + 
                                             grid[i][j][k-1] + grid[i][j][k+1] - 
                                             6 * grid[i][j][k]);
                            }
                        }
                    }
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