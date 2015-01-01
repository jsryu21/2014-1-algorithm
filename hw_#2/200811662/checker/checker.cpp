//Homework2 skeleton code

#include<cstdio>
#include<stdio.h>
#include<stdlib.h>

int OSInsert(int x, int* arr)
{
	int index = x - 1;
	if ( arr[ index ] != 0 )
	{
		// x is already in
		return 0;
	}
	arr[ index ] = 1;
	return x;
}

int OSDelete(int x, int* arr)
{
	int index = x - 1;
	if ( arr[ index ] == 0 )
	{
		// x not found.
		return 0;
	}
	arr[ index ] = 0;
	return x;
}

int OSSelect(int r, int* arr)
{
	int rank = 0;
	for (int i = 0; i < 1000; ++i)
	{
		if (arr[i] != 0)
		{
			// 0 is empty
			rank++;
			if (rank == r)
			{
				return i + 1;
			}
		}
	}
	return 0;
}

int OSRank(int x, int* arr)
{
	int index = x - 1;
	if ( arr[ index ] == 0 )
	{
		// x not found
		return 0;
	}
	int rank = 0;
	for ( int i = 0; i <= index; ++i )
	{
		if ( arr[ i ] != 0 )
		{
			// 0 is empty
			rank++;
		}
	}
	return rank;
}

int Operation(char Arg1, int Arg2, int* arr)
{
	int ret;
	switch (Arg1)
	{
	case 'I':
		ret = OSInsert(Arg2, arr);
		break;
	case 'D':
		ret = OSDelete(Arg2, arr);
		break;
	case 'S':
		ret = OSSelect(Arg2, arr);
		break;
	case 'R':
		ret = OSRank(Arg2, arr);
		break;
	default:
		//error
		break;
	}
	return ret;
}

//Check whether the output sequence is correct or not.
//If it is correct, return true. Otherwise, return false.
bool Checker(FILE* inFile, FILE* outFile)
{
	int arr[1000];
	for (int i = 0; i < 1000; ++i)
	{
		arr[i] = 0;
	}
	char Arg1;
	int Arg2;
	int Arg3;
	bool equal = true;
	// read file and check equal
	while (fscanf(inFile, "%c %d ", &Arg1, &Arg2) != EOF && fscanf(outFile, "%d ", &Arg3) != EOF)
	{
		if (Operation(Arg1, Arg2, arr) != Arg3)
		{
			equal = false;
			break;
		}
	}
	return equal;
}

int main(int argc, char* argv[])
{
	FILE* inFile = fopen(argv[1], "r");
	FILE* outFile = fopen(argv[2], "r");

	printf("%s\n", Checker(inFile, outFile) ? "YES" : "NO");

	fclose(outFile);
	fclose(inFile);
	return 0;
}

