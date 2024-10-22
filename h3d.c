#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100  // Grid size
#define T 500  // Number of time steps
#define ALPHA 0.01  // Diffusion coefficient

// Function to allocate 3D grid dynamically
double*** allocate_grid() {
    double*** grid = (double***)malloc(N * sizeof(double**));
    for (int i = 0; i < N; i++) {
        grid[i] = (double**)malloc(N * sizeof(double*));
        for (int j = 0; j < N; j++) {
            grid[i][j] = (double*)malloc(N * sizeof(double));
        }
    }
    return grid;
}

// Function to free dynamically allocated grid
void free_grid(double*** grid) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            free(grid[i][j]);
        }
        free(grid[i]);
    }
    free(grid);
}

// Initialize the grid
void initialize(double*** grid) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                grid[i][j][k] = 0.0;  // Initialize to room temperature
            }
        }
    }
    grid[N/2][N/2][N/2] = 100.0;  // Heat source in the center
}

void sequential_heat3D(double*** grid, double*** new_grid) {
    for (int t = 0; t < T; t++) {
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
        // Swap grids
        double*** temp = grid;
        grid = new_grid;
        new_grid = temp;
    }
}

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
        // Swap grids
        double*** temp = grid;
        grid = new_grid;
        new_grid = temp;
    }
}

int main() {
    double*** grid = allocate_grid();
    double*** new_grid = allocate_grid();

    initialize(grid);
    initialize(new_grid);

    // Sequential run
    double start_seq = omp_get_wtime();
    sequential_heat3D(grid, new_grid);
    double end_seq = omp_get_wtime();
    printf("Sequential time: %f seconds\n", end_seq - start_seq);

    initialize(grid);
    initialize(new_grid);

    // Parallel run
    double start_par = omp_get_wtime();
    parallel_heat3D(grid, new_grid);
    double end_par = omp_get_wtime();
    printf("Parallel time: %f seconds\n", end_par - start_par);

    // Free dynamically allocated memory
    free_grid(grid);
    free_grid(new_grid);

    return 0;
}
