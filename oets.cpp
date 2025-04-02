#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include "mpi.h"

using namespace std;

int main(int argc, char *argv[])
{
	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	// printf("I am %d of %d\n", rank, size);

	// vector<u_int8_t> bytes = getInputData();

	ifstream inputData("numbers", ios::binary);

	inputData.seekg(rank);
	u_int8_t num;
	inputData.read(reinterpret_cast<char *>(&num), 1);

	MPI_Barrier(MPI_COMM_WORLD);

	u_int8_t *allNums = nullptr;
	if (rank == 0)
	{
		allNums = new u_int8_t[size];
	}

	// Gather all bytes into rank 0
	MPI_Gather(&num, 1, MPI_UINT8_T, allNums, 1, MPI_UINT8_T, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d ", (int)allNums[i]);
		}
		printf("\n");
	}

	MPI_Barrier(MPI_COMM_WORLD);

	u_int8_t num2;

	for (int k = 1; k < 8; k++)
	{
		if (rank == 0 || rank == 2 || rank == 4 || rank == 6)
		{
			MPI_Send(&num, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD);
		}
		if (rank == 1 || rank == 3 || rank == 5 || rank == 7)
		{
			MPI_Send(&num, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD);
		}

		// MPI_Barrier(MPI_COMM_WORLD); dont think we need this one

		if (rank == 0 || rank == 2 || rank == 4 || rank == 6)
		{
			MPI_Recv(&num2, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD, &status);
			if (num > num2)
			{
				num = num2;
			}
		}
		if (rank == 1 || rank == 3 || rank == 5 || rank == 7)
		{
			MPI_Recv(&num2, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD, &status);
			if (num < num2)
			{
				num = num2;
			}
		}

		MPI_Barrier(MPI_COMM_WORLD); // not sure

		if (rank == 1 || rank == 3 || rank == 5)
		{
			MPI_Send(&num, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD);
		}
		if (rank == 2 || rank == 4 || rank == 6)
		{
			MPI_Send(&num, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD);
		}
		if (rank == 1 || rank == 3 || rank == 5)
		{
			MPI_Recv(&num2, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD, &status);
			if (num > num2)
			{
				num = num2;
			}
		}
		if (rank == 2 || rank == 4 || rank == 6)
		{
			MPI_Recv(&num2, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD, &status);
			if (num < num2)
			{
				num = num2;
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	/* for (int i = 0; i < size; i++)
	{
		if (i == rank)
		{
			printf("I am %d of %d: %d\n", rank, size, num);
			fflush(stdout);
		}
		MPI_Barrier(MPI_COMM_WORLD);
	} */

	MPI_Gather(&num, 1, MPI_UINT8_T, allNums, 1, MPI_UINT8_T, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d\n", (int)allNums[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
}
