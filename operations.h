#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "template.h"

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
    grid[N/2][N/2][N/2] = TEMP_SOURCE;  // Heat source in the center
}

// Print the grid for debugging or viewing final state
void print_grid(double*** grid) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                if (i==50 && j==51 && k==50)
                printf("(%d, %d, %d): %f ", i, j, k, grid[i][j][k]);
            }
            // printf("\n");
        }
        // printf("\n");
    }
}

#endif