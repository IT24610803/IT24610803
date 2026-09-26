#include <mpi.h>
#include <iostream>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 10000000;

    long long base = N / size;
    long long remainder = N % size;

    long long start;
    long long end;

    if (rank < remainder)
    {
        start = rank * (base + 1) + 1;
        end = start + base;
    }
    else
    {
        start = remainder * (base + 1)
              + (rank - remainder) * base + 1;

        end = start + base - 1;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    double startTime = MPI_Wtime();

    long long localSum = 0;

    for (long long i = start; i <= end; i++)
    {
        localSum += i;
    }

    long long totalSum = 0;

    MPI_Reduce(
        &localSum,
        &totalSum,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    double endTime = MPI_Wtime();

    if (rank == 0)
    {
        std::cout << "Number of processes: "
                  << size << std::endl;

        std::cout << "Total sum: "
                  << totalSum << std::endl;

        std::cout << "Execution time: "
                  << endTime - startTime
                  << " seconds" << std::endl;
    }

    MPI_Finalize();

    return 0;
}
