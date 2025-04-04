/*
VUT FIT - PRL projekt 1
Timotej Halenár - xhalen00
3.4.2024
*/

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

	// input file handling
	ifstream inputData("numbers", ios::binary);

	// find the byte in the file equal to the rank and read it into variable
	inputData.seekg(rank);
	u_int8_t num;
	inputData.read(reinterpret_cast<char *>(&num), 1);
	inputData.close();

	// variable for all numbers
	u_int8_t *allNums = nullptr;
	// only allocate memory if in process n. 0
	if (rank == 0)
	{
		allNums = new u_int8_t[size];
	}

	// gather all numbers, so they can be printed
	MPI_Gather(&num, 1, MPI_UINT8_T, allNums, 1, MPI_UINT8_T, 0, MPI_COMM_WORLD);
	// print numbers from one process
	if (rank == 0)
	{
		for (int i = 0; i < size; i++)
		{
			printf("%d ", (int)allNums[i]);
		}
		printf("\n");
	}

	// MPI_Barrier(MPI_COMM_WORLD); I guess we don't need this one?

	// var for recieving numbers from neighbors
	u_int8_t num2;

	// main algorithm loop
	for (int k = 1; k < size; k++)
	{

		// 0-1, 2-3, 4-5, 6-7
		// 'even' phase (but actually odd phase because it goes from 0)
		// exchange number with above neighbor
		for (int i = 0; i <= 2 * (size / 2) - 1; i += 2)
		{
			if (i == rank)
			{
				MPI_Sendrecv(
					&num, 1, MPI_UINT8_T, rank + 1, 0,
					&num2, 1, MPI_UINT8_T, rank + 1, 0,
					MPI_COMM_WORLD, &status);
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
				MPI_Sendrecv(
					&num, 1, MPI_UINT8_T, rank - 1, 0,
					&num2, 1, MPI_UINT8_T, rank - 1, 0,
					MPI_COMM_WORLD, &status);
				if (num < num2)
				{
					num = num2;
				}
			}
		}

		// 1-2, 3-4, 5-6
		// 'odd' phase
		// exchange number with below neighbor
		for (int i = 1; i < 2 * ((size - 1) / 2); i += 2)
		{
			if (i == rank)
			{
				MPI_Sendrecv(
					&num, 1, MPI_UINT8_T, rank + 1, 0,
					&num2, 1, MPI_UINT8_T, rank + 1, 0,
					MPI_COMM_WORLD, &status);
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
				MPI_Sendrecv(
					&num, 1, MPI_UINT8_T, rank - 1, 0,
					&num2, 1, MPI_UINT8_T, rank - 1, 0,
					MPI_COMM_WORLD, &status);
				if (num < num2)
				{
					num = num2;
				}
			}
		}
	}

	// MPI_Barrier(MPI_COMM_WORLD); we don't need this one either??

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
