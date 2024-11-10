#ifndef DOMAIN_H
#define DOMAIN_H

#include "template.h"
#include <math.h>
// Domain-decomposed parallel 3D heat conduction function
void domain_decomposed_heat3D(double*** grid, double*** new_grid) {
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_threads = omp_get_num_threads();

        // Calculate sub-domain bounds for each thread
        int sub_domains_per_dim = (int)sqrt(num_threads); // Split threads evenly in a 2D grid
        int sub_size = N / sub_domains_per_dim; // Size of each sub-domain
        int i_start = (tid / sub_domains_per_dim) * sub_size;
        int i_end = i_start + sub_size;
        int j_start = (tid % sub_domains_per_dim) * sub_size;
        int j_end = j_start + sub_size;

        // Ensure bounds do not exceed the grid dimensions
        if (i_end > N) i_end = N;
        if (j_end > N) j_end = N;

        for (int t = 0; t < T; t++) {
            // Update sub-domain assigned to each thread
            for (int i = i_start + 1; i < i_end - 1; i++) {
                for (int j = j_start + 1; j < j_end - 1; j++) {
                    for (int k = 1; k < N - 1; k++) {
                        new_grid[i][j][k] = grid[i][j][k] +
                            ALPHA * (grid[i-1][j][k] + grid[i+1][j][k] + 
                                     grid[i][j-1][k] + grid[i][j+1][k] + 
                                     grid[i][j][k-1] + grid[i][j][k+1] - 
                                     6 * grid[i][j][k]);
                    }
                }
            }

            // Synchronize boundary data across threads
            #pragma omp barrier
            #pragma omp for collapse(2)
            for (int i = i_start; i < i_end; i++) {
                for (int j = j_start; j < j_end; j++) {
                    // Update the boundaries of each sub-domain for next iteration
                    if (i == i_start || i == i_end - 1 || j == j_start || j == j_end - 1) {
                        for (int k = 1; k < N - 1; k++) {
                            new_grid[i][j][k] = grid[i][j][k] +
                                ALPHA * (grid[i-1][j][k] + grid[i+1][j][k] +
                                         grid[i][j-1][k] + grid[i][j+1][k] +
                                         grid[i][j][k-1] + grid[i][j][k+1] -
                                         6 * grid[i][j][k]);
                        }
                    }
                }
            }

            // Swap grids after each time step
            #pragma omp single
            {
                double*** temp = grid;
                grid = new_grid;
                new_grid = temp;
            }

            #pragma omp barrier
        }
    }
}

#endif
