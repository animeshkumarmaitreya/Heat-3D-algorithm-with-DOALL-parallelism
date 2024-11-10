// #ifndef TEMPORAL_H
// #define TEMPORAL_H

// #include "template.h"

// void temporal_parallel_heat3D(double*** grid, double*** new_grid) {
//     for (int t = 0; t < T; t++) {
//         #pragma omp parallel for collapse(3) schedule(dynamic) //static check krlena
//         for (int wave = 0; wave < N / TILE_SIZE; wave++) {
//             for (int i = wave * TILE_SIZE; i < (wave + 1) * TILE_SIZE && i < N - 1; i++) {
//                 for (int j = wave * TILE_SIZE; j < (wave + 1) * TILE_SIZE && j < N - 1; j++) {
//                     for (int k = wave * TILE_SIZE; k < (wave + 1) * TILE_SIZE && k < N - 1; k++) {
//                         if (i > 0 && j > 0 && k > 0 && i < N-1 && j < N-1 && k < N-1) {
//                             new_grid[i][j][k] = grid[i][j][k] +
//                                 ALPHA * (grid[i-1][j][k] + grid[i+1][j][k] + grid[i][j-1][k] + 
//                                          grid[i][j+1][k] + grid[i][j][k-1] + grid[i][j][k+1] -
//                                          6 * grid[i][j][k]);
//                         }
//                     }                }
//             }
//         }
//         // Swap pointers instead of copying grids
//         double*** temp = grid;
//         grid = new_grid;
//         new_grid = temp;
//     }
// }

// #endif