/*
 * GitHub Repository: https://github.com/Mhdi57/Lab_proj
 */
#include <mpi.h>
#include <stdio.h>

#define TOKEN_TAG 100

int main(int argc, char *argv[])
{
    int rank, size;
    int token = 1;
    int successor, predecessor;
    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3)
    {
        if (rank == 0)
        {
            printf("This program must be run with exactly 3 processes.\n");
            printf("Use: mpiexec -n 3 ./token_ring.exe\n");
        }

        MPI_Finalize();
        return 0;
    }

    successor = (rank + 1) % size;
    predecessor = (rank - 1 + size) % size;

    printf("Process %d is requesting access to the printer.\n", rank);

    if (rank == 0)
    {
        printf("Process %d has the token.\n", rank);
        printf("Process %d is printing using the shared printer.\n", rank);
        printf("Process %d released the printer.\n", rank);

        MPI_Send(&token, 1, MPI_INT, successor, TOKEN_TAG, MPI_COMM_WORLD);
        printf("Process %d sent the token to Process %d.\n", rank, successor);

        MPI_Recv(&token, 1, MPI_INT, predecessor, TOKEN_TAG, MPI_COMM_WORLD, &status);
        printf("Process %d received the token back from Process %d.\n", rank, predecessor);
        printf("Token circulation completed successfully.\n");
    }
    else
    {
        MPI_Recv(&token, 1, MPI_INT, predecessor, TOKEN_TAG, MPI_COMM_WORLD, &status);

        printf("Process %d received the token from Process %d.\n", rank, predecessor);
        printf("Process %d is printing using the shared printer.\n", rank);
        printf("Process %d released the printer.\n", rank);

        MPI_Send(&token, 1, MPI_INT, successor, TOKEN_TAG, MPI_COMM_WORLD);
        printf("Process %d sent the token to Process %d.\n", rank, successor);
    }

    MPI_Finalize();

    return 0;
}