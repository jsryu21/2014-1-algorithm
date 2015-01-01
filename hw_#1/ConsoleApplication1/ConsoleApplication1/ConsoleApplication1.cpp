#include "stdafx.h"
//#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

static inline long myclock()
{
	//struct timeval tv;
	//gettimeofday(&tv, NULL);
	//return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
	return 0;
}

struct Matrix
{
	Matrix(unsigned int size)
	:size(size)
	{
		datas = (long long**)malloc(sizeof(long long*)*size);
		for (unsigned int i = 0; i < size; ++i) {
			datas[i] = (long long*)malloc(sizeof(long long)*size);
		}
	}
	~Matrix()
	{
		for (unsigned int i = 0; i < size; ++i) {
			free(datas[i]);
		}
		free(datas);
	}
	void Print(std::ostream& out) const
	{
		for (unsigned int i = 0; i < size; ++i) {
			for (unsigned int j = 0; j < size; ++j) {
				out << datas[i][j] << " ";
			}
			out << std::endl;
		}
	}
	long long** datas;
	unsigned int size;
};

class MatrixMultiplication
{
protected:
	MatrixMultiplication(Matrix& mA, Matrix& mB, Matrix& mC, unsigned int size, unsigned int a1 = 0, unsigned int a2 = 0, unsigned int b1 = 0, unsigned int b2 = 0)
		:mA(mA)
		, mB(mB)
		, mC(mC)
		, size(size)
		, a1(a1)
		, a2(a2)
		, b1(b1)
		, b2(b2)
	{
	}
	virtual void Multiply() = 0;
	Matrix& mA;
	Matrix& mB;
	Matrix& mC;
	unsigned int size;
	unsigned int a1;
	unsigned int a2;
	unsigned int b1;
	unsigned int b2;
};

class UsualMatrixMultiplication : public MatrixMultiplication
{
public:
	UsualMatrixMultiplication(Matrix& mA, Matrix& mB, Matrix& mC, unsigned int size)
		:MatrixMultiplication(mA, mB, mC, size)
	{
	}
	void Multiply()
	{
		for (unsigned int i = 0; i < size; ++i) {
			for (unsigned int j = 0; j < size; ++j) {
				long long sum = 0;
				for (unsigned int k = 0; k < size; ++k) {
					sum += mA.datas[a1 + i][a2 + k] * mB.datas[b1 + k][b2 + j];
				}
				mC.datas[i][j] = sum;
			}
		}
	}
};

class StrassenMatrixMultiplication : public MatrixMultiplication
{
public:
	StrassenMatrixMultiplication(Matrix& mA, Matrix& mB, Matrix& mC, unsigned int size, unsigned int a1 = 0, unsigned int a2 = 0, unsigned int b1 = 0, unsigned int b2 = 0)
		:MatrixMultiplication(mA, mB, mC, size, a1, a2, b1, b2)
	{
	}
	void Multiply()
	{
		if (size == 1)
		{
			mC.datas[0][0] = mA.datas[a1][a2] * mB.datas[b1][b2];
			return;
		}
		unsigned int halfSize = size / 2;
		Matrix p1(halfSize);
		Matrix s1(halfSize);
		Matrix s2(halfSize);
		{
			for (unsigned int i = 0, hB1 = b1 + i, tB1 = b1 + i + halfSize; i < halfSize; ++i, ++hB1, ++tB1) {
				for (unsigned int j = 0, bB2 = b2 + j + halfSize; j < halfSize; ++j, ++bB2) {
					s1.datas[i][j] = mB.datas[hB1][bB2] - mB.datas[tB1][bB2];
				}
			}
			StrassenMatrixMultiplication(mA, s1, p1, halfSize, a1, a2).Multiply();
		}
		Matrix p2(halfSize);
		{
			for (unsigned int i = 0, bA1 = a1 + i; i < halfSize; ++i, ++bA1) {
				for (unsigned int j = 0, hA2 = a2 + j, tA2 = a2 + j + halfSize; j < halfSize; ++j, ++hA2, ++tA2) {
					s1.datas[i][j] = mA.datas[bA1][hA2] + mA.datas[bA1][tA2];
				}
			}
			StrassenMatrixMultiplication(s1, mB, p2, halfSize, 0, 0, b1 + halfSize, b2 + halfSize).Multiply();
		}
		Matrix p3(halfSize);
		{
			for (unsigned int i = 0, bA1 = a1 + i + halfSize; i < halfSize; ++i, ++bA1) {
				for (unsigned int j = 0, hA2 = a2 + j, tA2 = a2 + j + halfSize; j < halfSize; ++j, ++hA2, ++tA2) {
					s1.datas[i][j] = mA.datas[bA1][hA2] + mA.datas[bA1][tA2];
				}
			}
			StrassenMatrixMultiplication(s1, mB, p3, halfSize, 0, 0, b1, b2).Multiply();
		}
		Matrix p4(halfSize);
		{
			for (unsigned int i = 0, hB1 = b1 + i + halfSize, tB1 = b1 + i; i < halfSize; ++i, ++hB1, ++tB1) {
				for (unsigned int j = 0, bB2 = b2 + j; j < halfSize; ++j, ++bB2) {
					s1.datas[i][j] = mB.datas[hB1][bB2] - mB.datas[tB1][bB2];
				}
			}
			StrassenMatrixMultiplication(mA, s1, p4, halfSize, a1 + halfSize, a2 + halfSize).Multiply();
		}
		Matrix p5(halfSize);
		{
			for (unsigned int i = 0, hA1 = a1 + i, tA1 = a1 + i + halfSize; i < halfSize; ++i, ++hA1, ++tA1) {
				for (unsigned int j = 0, hA2 = a2 + j, tA2 = a2 + j + halfSize; j < halfSize; ++j, ++hA2, ++tA2) {
					s1.datas[i][j] = mA.datas[hA1][hA2] + mA.datas[tA1][tA2];
				}
			}
			for (unsigned int i = 0, hB1 = b1 + i, tB1 = b1 + i + halfSize; i < halfSize; ++i, ++hB1, ++tB1) {
				for (unsigned int j = 0, hB2 = b2 + j, tB2 = b2 + j + halfSize; j < halfSize; ++j, ++hB2, ++tB2) {
					s2.datas[i][j] = mB.datas[hB1][hB2] + mB.datas[tB1][tB2];
				}
			}
			StrassenMatrixMultiplication(s1, s2, p5, halfSize).Multiply();
		}
		Matrix p6(halfSize);
		{
			for (unsigned int i = 0, hA1 = a1 + i, tA1 = a1 + i + halfSize; i < halfSize; ++i, ++hA1, ++tA1) {
				for (unsigned int j = 0, bA2 = a2 + j + halfSize; j < halfSize; ++j, ++bA2) {
					s1.datas[i][j] = mA.datas[hA1][bA2] - mA.datas[tA1][bA2];
				}
			}
			for (unsigned int i = 0, bB1 = b1 + i + halfSize; i < halfSize; ++i, ++bB1) {
				for (unsigned int j = 0, hB2 = b2 + j, tB2 = b2 + j + halfSize; j < halfSize; ++j, ++hB2, ++tB2) {
					s2.datas[i][j] = mB.datas[bB1][hB2] + mB.datas[bB1][tB2];
				}
			}
			StrassenMatrixMultiplication(s1, s2, p6, halfSize).Multiply();
		}
		Matrix p7(halfSize);
		{
			for (unsigned int i = 0, hA1 = a1 + i, tA1 = a1 + i + halfSize; i < halfSize; ++i, ++hA1, ++tA1) {
				for (unsigned int j = 0, bA2 = a2 + j; j < halfSize; ++j, ++bA2) {
					s1.datas[i][j] = mA.datas[hA1][bA2] - mA.datas[tA1][bA2];
				}
			}
			for (unsigned int i = 0, bB1 = b1 + i; i < halfSize; ++i, ++bB1) {
				for (unsigned int j = 0, hB2 = b2 + j, tB2 = b2 + j + halfSize; j < halfSize; ++j, ++hB2, ++tB2) {
					s2.datas[i][j] = mB.datas[bB1][hB2] + mB.datas[bB1][tB2];
				}
			}
			StrassenMatrixMultiplication(s1, s2, p7, halfSize).Multiply();
		}
		for (unsigned int i = 0, c1 = i + halfSize; i < halfSize; ++i, ++c1) {
			for (unsigned int j = 0, c2 = j + halfSize; j < halfSize; ++j, ++c2) {
				mC.datas[i][j] = p5.datas[i][j] + p4.datas[i][j] - p2.datas[i][j] + p6.datas[i][j];
				mC.datas[i][c2] = p1.datas[i][j] + p2.datas[i][j];
				mC.datas[c1][j] = p3.datas[i][j] + p4.datas[i][j];
				mC.datas[c1][c2] = p5.datas[i][j] + p1.datas[i][j] - p3.datas[i][j] - p7.datas[i][j];
			}
		}
	}
};

int main(int argc, char * argv[])
{
	std::string inputFileName = "input.txt";
	std::string outputFileName = "200811662.txt";
	if (argc == 2) {
		inputFileName = argv[1];
	}
	else if (argc == 3) {
		inputFileName = argv[1];
		outputFileName = argv[2];
	}
	std::ifstream inputFile;
	inputFile.open(inputFileName.c_str(), std::fstream::in);
	if (inputFile.is_open() == false) {
		std::cout << "Input file not found. File name : " << inputFileName << std::endl;
		return -1;
	}
	std::string line;
	if (!std::getline(inputFile, line)) {
		std::cout << "Invalid matrix size." << std::endl;
		return -1;
	}
	unsigned int size = atoi(line.c_str());
	Matrix mA(size);
	Matrix mB(size);
	for (unsigned int i = 0; i < size; ++i) {
		if (!std::getline(inputFile, line)) {
			std::cout << "Invalid matrix A" << std::endl;
			return -1;
		}
		std::istringstream iss(line);
		for (unsigned int j = 0; j < size && iss; ++j) {
			std::string sub;
			iss >> sub;
			mA.datas[i][j] = atoll(sub.c_str());
		}
	}
	for (unsigned int i = 0; i < size; ++i) {
		if (!std::getline(inputFile, line)) {
			std::cout << L"Invalid matrix B" << std::endl;
			return -1;
		}
		std::istringstream iss(line);
		for (unsigned int j = 0; j < size && iss; ++j) {
			std::string sub;
			iss >> sub;
			mB.datas[i][j] = atoll(sub.c_str());
		}
	}
	//mA.Print(std::cout);
	//mB.Print(std::cout);
	Matrix mC(size);
	long t, dt;
	t = myclock();
	UsualMatrixMultiplication(mA, mB, mC, mA.size).Multiply();
	dt = myclock() - t;
	std::cout << "Usual matrix multiplication. Size : " << mA.size << ", time : " << dt << std::endl;
	//mC.Print(std::cout);
	Matrix mD(size);
	t = myclock();
	StrassenMatrixMultiplication(mA, mB, mD, mA.size).Multiply();
	dt = myclock() - t;
	std::cout << "Strassen matrix multiplication. Size : " << mA.size << ", time : " << dt << std::endl;
	//mD.Print(std::cout);
	bool isSame = true;
	for (unsigned int i = 0; i < size && isSame; ++i){
		for (unsigned int j = 0; j < size && isSame; ++j){
			if (mC.datas[i][j] != mD.datas[i][j]){
				std::cout << "Multiplication results are not same." << std::endl;
				isSame = false;
			}
		}
	}
	std::ofstream outputFile;
	outputFile.open(outputFileName.c_str());
	if (outputFile.is_open() == false){
		std::cout << "Output file not found. File name : " << outputFileName << std::endl;
	}
	if (isSame){
		mC.Print(outputFile);
		outputFile << "Yes" << std::endl;
	}
	else{
		mC.Print(outputFile);
		mD.Print(outputFile);
		outputFile << "No" << std::endl;
	}
	outputFile.close();
	return 0;
}