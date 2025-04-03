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

	ifstream inputData("numbers", ios::binary);

	inputData.seekg(rank);
	u_int8_t num;
	inputData.read(reinterpret_cast<char *>(&num), 1);
	inputData.close();

	u_int8_t *allNums = nullptr;
	if (rank == 0)
	{
		allNums = new u_int8_t[size];
	}
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

	for (int k = 1; k < size; k++)
	{
		// 0-1, 2-3, 4-5, 6-7
		// send your number to neighbor
		for (int i = 0; i <= 2 * (size / 2) - 1; i += 2)
		{
			if (i == rank)
				MPI_Send(&num, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD);
		}
		for (int i = 1; i <= 2 * (size / 2) - 1; i += 2)
		{
			if (i == rank)
				MPI_Send(&num, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD);
		}

		// recieve number from neighbor
		for (int i = 0; i <= 2 * (size / 2) - 1; i += 2)
		{
			if (i == rank)
			{
				MPI_Recv(&num2, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD, &status);
				if (num > num2)
				{
					num = num2;
				}
			}
		}
		for (int i = 1; i <= 2 * (size / 2) - 1; i += 2)
		{
			if (i == rank)
			{
				MPI_Recv(&num2, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD, &status);
				if (num < num2)
				{
					num = num2;
				}
			}
		}

		// MPI_Barrier(MPI_COMM_WORLD); // not sure

		// 1-2, 3-4, 5-6
		// send your number to neighbor
		for (int i = 1; i < 2 * ((size - 1) / 2); i += 2)
		{
			if (i == rank)
				MPI_Send(&num, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD);
		}
		for (int i = 2; i <= 2 * ((size - 1) / 2); i += 2)
		{
			if (i == rank)
				MPI_Send(&num, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD);
		}

		// recieve number from neighbor
		for (int i = 1; i < 2 * ((size - 1) / 2); i += 2)
		{
			if (i == rank)
			{
				MPI_Recv(&num2, 1, MPI_UINT8_T, rank + 1, 0, MPI_COMM_WORLD, &status);
				if (num > num2)
				{
					num = num2;
				}
			}
		}
		for (int i = 2; i <= 2 * ((size - 1) / 2); i += 2)
		{
			if (i == rank)
			{
				MPI_Recv(&num2, 1, MPI_UINT8_T, rank - 1, 0, MPI_COMM_WORLD, &status);
				if (num < num2)
				{
					num = num2;
				}
			}
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Gather(&num, 1, MPI_UINT8_T, allNums, 1, MPI_UINT8_T, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d\n", (int)allNums[i]);
		}
	}

	MPI_Finalize();
}
