#ifndef BINARY_HEAP_HPP
#define BINARY_HEAP_HPP

class BinaryHeap
{
private:
	int **reverseMap;
	double * values;
	int **correspondingIndexes;
	int size, matrixSizeOfConstruction;
	void siftUp(int idx);
	void sinkDown(int idx);
	void swapIdxs(int idx1, int idx2);

public:
	BinaryHeap(double **dists, int matrixSize);
	~BinaryHeap();
	void modifyKey(int x, int y, double newValue);
	void removeElement(int x, int y);

	double minDist()const;
	int minDistX()const;
	int minDistY()const;
};
#endif //BINARY_HEAP_HPP
