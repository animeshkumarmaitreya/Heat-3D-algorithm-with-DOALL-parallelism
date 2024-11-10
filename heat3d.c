#include "template.h"
#include "operations.h"

#include "sequential.h" //-s
#include "doall.h" //-d
#include "tiling.h" //-t
#include "wavefront.h" //-w
#include "domain_decomposition.h" //-dd
// #include "temporal_decomposition.h" //-td
#include "numa.h" //-n

int main(int argc, char *argv[]) {
    
    int runSequential = 0;
    int runDoAll = 0;
    int runTiling = 0;
    int runWavefront = 0;
    int runDomainDecomposition = 0;
    int runTemporalDecomposition = 0;
    int runNUMA = 0;

    double sequential_time = 0;
    double numa_time = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            runSequential = 1;
        } else if (strcmp(argv[i], "-d") == 0) {
            runDoAll = 1;
        } else if (strcmp(argv[i], "-t") == 0) {
            runTiling = 1;
        } else if (strcmp(argv[i], "-w") == 0) {
            runWavefront = 1;
        } else if (strcmp(argv[i], "-dd") == 0) {
            runDomainDecomposition = 1;
        } else if (strcmp(argv[i], "-td") == 0) {
            runTemporalDecomposition = 1;
        } else if (strcmp(argv[i], "-n") == 0) {
            runNUMA = 1;
        } else {
            printf("Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    if(runNUMA == 0)
    {
        double*** grid = allocate_grid();
        double*** new_grid = allocate_grid();

        initialize(grid);
        initialize(new_grid);

        if(runSequential){
            // Sequential run
            double start_seq = omp_get_wtime();
            sequential_heat3D(grid, new_grid);
            double end_seq = omp_get_wtime();
            sequential_time = end_seq - start_seq;
            printf("Sequential time: %f seconds\n", sequential_time);
        }
        if(runDoAll){
            // Parallel run
            double start_par = omp_get_wtime();
            parallel_heat3D(grid, new_grid);
            double end_par = omp_get_wtime();
            printf("Parallel time: %f seconds\n", end_par - start_par);
            if(runSequential){
                printf("DOALL Speedup: %f\n", sequential_time/(end_par-start_par));
            }
        }
        if(runTiling){
            // Tiling run
            double start_tiled = omp_get_wtime();
            tiled_heat3D(grid, new_grid);
            double end_tiled = omp_get_wtime();
            printf("Tiled parallel time: %f seconds\n", end_tiled - start_tiled);
            if(runSequential){
                printf("Tiling Speedup: %f\n", sequential_time/(end_tiled-start_tiled));
            }
        }
        if(runWavefront){
            // Wavefront parallel run
            double start_wavefront = omp_get_wtime();
            wavefront_heat3D(grid, new_grid);
            double end_wavefront = omp_get_wtime();
            printf("Wavefront parallel time: %f seconds\n", end_wavefront - start_wavefront);
            if(runSequential){
                printf("Wavefront Speedup: %f\n", sequential_time/(end_wavefront-start_wavefront));
            }
        }
        if(runDomainDecomposition){
            // Domain decomposition run
            double start_par = omp_get_wtime();
            domain_decomposed_heat3D(grid, new_grid);
            double end_par = omp_get_wtime();
            printf("Domain-decomposed parallel time: %f seconds\n", end_par - start_par);
            if(runSequential){
                printf("Domain Decomposed Speedup: %f\n", sequential_time/(end_par-start_par));
            }
        }
        // if(runTemporalDecomposition){
        //     // Temporal decomposition run
        //     double start_time = omp_get_wtime();
        //     temporal_parallel_heat3D(grid, new_grid);
        //     double end_time = omp_get_wtime();
        //     printf("Temporal parallel time: %f seconds\n", end_time - start_time);
        //     if(runSequential){
        //         printf("Temporal Decomposed Speedup: %f\n", sequential_time/(end_time-start_time));
        //     }
        // }
        print_grid(grid);
        free_grid(grid);
        free_grid(new_grid);
    }
    else if(runNUMA)
    {
        // Allocate memory on NUMA node 0 for demonstration (modify as needed)
        double*** grid = allocate_grid_numa(0);
        double*** new_grid = allocate_grid_numa(0);

        initialize(grid);
        initialize(new_grid);

        // Measure the time for NUMA-aware parallel execution
        double start = omp_get_wtime();
        numa_heat3D(grid, new_grid);
        double end = omp_get_wtime();
        numa_time = end - start;

        //NUMA print
        if(runSequential){
            printf("Sequential time: %f seconds\n", sequential_time);
        }
        printf("NUMA-aware parallel time: %f seconds\n",numa_time);
        if(runSequential){
            printf("NUMA-aware Speedup: %f\n", sequential_time/numa_time);
        }

        // Print the final state of the grid (optional)
        // print_grid(grid);

        // Free dynamically allocated memory
        print_grid(grid);

        free_grid_numa(grid);
        free_grid_numa(new_grid);
    }
    return 0;
}
