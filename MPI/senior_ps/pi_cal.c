#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int n, r;

    MPI_Comm_size(MPI_COMM_WORLD, &n);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);

    int N;
    double start_time, end_time, seq_start, seq_end;

    if (r == 0) {
        printf("Enter the number of rectangles:\n");
        scanf("%d", &N);

        // Sequential execution timing
        seq_start = MPI_Wtime();
        double seq_pi = 0.0;
        double step = 1.0 / N;
        for (int i = 0; i < N; i++) {
            double x = (i + 0.5) * step;
            seq_pi += 4.0 / (1.0 + x * x);
        }
        seq_pi *= step;
        seq_end = MPI_Wtime();
    }

    // Start timing for parallel execution
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    int base = N / n;
    double step = 1.0 / N;
    double par_pi = 0, pi;

    for (int i = 0; i < base; i++) {
        double x = (r * base + i + 0.5) * step;
        par_pi += 4.0 / (1.0 + x * x);
    }

    printf("Process %d part = %lf\n", r, par_pi);

    // Reduce partial results
    MPI_Reduce(&par_pi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    end_time = MPI_Wtime();  // Stop timing

    if (r == 0) {
        printf("Final pi value = %.10lf\n", pi * step);
        printf("Sequential Execution Time: %.6f seconds\n", seq_end - seq_start);
        printf("Parallel Execution Time: %.6f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
