#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{
	ifstream inputData("numbers", ios::binary);

	vector<u_int8_t> bytes(
		(istreambuf_iterator<char>(inputData)),
		(istreambuf_iterator<char>()));

	inputData.close();

	for (int i = 0; i < bytes.size(); i++)
	{
		cout << (int)bytes[i] << " ";
	}
	cout << endl;
}
