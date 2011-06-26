#include "binary_heap.h"
#include "constants.h"

#include <iostream>
using namespace std;

void BinaryHeap::swapIdxs(int idx1, int idx2)
{
	swap(values[idx1], values[idx2]);
	swap(correspondingIndexes[idx1][0], correspondingIndexes[idx2][0]);
	swap(correspondingIndexes[idx1][1], correspondingIndexes[idx2][1]);
	reverseMap[correspondingIndexes[idx1][0]][correspondingIndexes[idx1][1]] = idx1;
	reverseMap[correspondingIndexes[idx2][0]][correspondingIndexes[idx2][1]] = idx2;
}

void BinaryHeap::siftUp(int idx)
{
	if(idx == 1 || values[idx] >= values[idx / 2])
	{
		return;
	}
	
	swapIdxs(idx, idx / 2);
	siftUp(idx / 2);
}

void BinaryHeap::sinkDown(int idx)
{
	if(idx * 2 > size)
		return; 
	double minimal = values[idx * 2];
	int minIdx = idx * 2;
	if(idx * 2 + 1 <= size && minimal > values[idx * 2 + 1])
	{
		minimal = values[idx * 2 + 1];
		minIdx = idx * 2 + 1;
	}
	if(values[idx] > minimal)
	{
		swapIdxs(idx, minIdx);
		sinkDown(minIdx);
	}
}

BinaryHeap::BinaryHeap(double **dists, int matrixSize)
{
	int heapSize = matrixSize * matrixSize + 1;

	correspondingIndexes = new int* [heapSize];
	for(int i = 0; i < heapSize; i++)
		correspondingIndexes[i] = new int[2];

	reverseMap = new int* [matrixSize];
	for(int i = 0; i < matrixSize; i++)
		reverseMap[i] = new int[matrixSize];

	values = new double [heapSize];

	int idx;
	for(int i = 0; i < matrixSize; i++)
	{
		for(int j = 0; j < matrixSize; j++)
		{
			idx = i * matrixSize + j + 1;

			values[idx] = dists[i][j];
			correspondingIndexes[idx][0] = i;
			correspondingIndexes[idx][1] = j;
			reverseMap[i][j] = idx;
		}
	}

	matrixSizeOfConstruction = matrixSize;
	size = heapSize - 1;
	for(int i = size; i > 0; i--)
		sinkDown(i);
}

BinaryHeap::~BinaryHeap()
{
	delete [] values;

	for(int i = 0; i < matrixSizeOfConstruction; i++)
		delete [] reverseMap[i];
	delete [] reverseMap;

	int lim = matrixSizeOfConstruction * matrixSizeOfConstruction + 1;
	for(int i = 0; i < lim; i++)
		delete [] correspondingIndexes[i];
	delete [] correspondingIndexes;
}

void BinaryHeap::modifyKey(int x, int y, double newValue)
{
	int idx = reverseMap[x][y];
	if(newValue > values[idx])
	{
		values[idx] = newValue;
		sinkDown(idx);
	}
	else
	{
		values[idx] = newValue;
		siftUp(idx);
	}
}

void BinaryHeap::removeElement(int x, int y)
{
	int idx = reverseMap[x][y];
	double tmpVal = values[idx];
	swapIdxs(idx, size);
	values[size] = INFINITY;
	size--;
	swap(tmpVal, values[idx]);
	modifyKey(correspondingIndexes[idx][0], correspondingIndexes[idx][1], tmpVal);
}

double BinaryHeap::minDist()const
{
	return values[1];
}

int BinaryHeap::minDistX()const
{
	return correspondingIndexes[1][0];
}

int BinaryHeap::minDistY()const
{
	return correspondingIndexes[1][1];
}
