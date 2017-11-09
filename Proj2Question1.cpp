// Bony Patel
// Parallel and Distributed Programming Project 2 Question 1

#include "stdafx.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <algorithm>
#include <chrono>
#include <future>
#include <fstream>
#include <sstream>
#include <random>

using namespace std;

// generate a random array of numbers from 0 to 10000
vector<int> generateArray(int size, int low, int high) {
	vector<int> arr;

	for (int i = 0; i < size; i++)
	{
		arr.push_back(rand() % high + low);
	}

	return arr;
}

int quickSortSerial(vector<int>::iterator begin, vector<int>::iterator end) {
	auto const sz = end - begin;
	if (sz <= 1) return 0;

	auto pivot = begin + sz / 2;
	auto const pivot_v = *pivot;

	swap(*pivot, *(end - 1));
	auto p = partition(begin, end, [&](const int& a) { return a < pivot_v; });
	swap(*p, *(end - 1));

	if (false) {
		auto left = std::async(std::launch::async, [&]() {
			return quickSortSerial(begin, p);
		});
		quickSortSerial(p + 1, end);
	}
	else {
		quickSortSerial(begin, p);
		quickSortSerial(p + 1, end);
	}
	return 0;
}

void quickSortSerialRun(vector<int>& arr) {
	quickSortSerial(arr.begin(), arr.end());
}

// Parallel Quick Sort definition
int quickSortParallel(vector<int>::iterator begin, vector<int>::iterator end, int threads) {
	auto const sz = end - begin;
	if (sz <= 1) return 0;

	auto pivot = begin + sz / 2;
	auto const pivot_v = *pivot;

	swap(*pivot, *(end - 1));
	auto p = partition(begin, end, [&](const int& a) { return a < pivot_v; });
	swap(*p, *(end - 1));

	if (sz > 4096 - (threads^32)) {
		auto left = std::async(std::launch::async, [&]() {
			
			return quickSortParallel(begin, p, 0);
		});
		quickSortParallel(p + 1, end, 0);
	}
	else {
		quickSortParallel(begin, p, threads);
		quickSortParallel(p + 1, end, threads);
	}
	return 0;
}

void quickSortParallelRun(vector<int>& arr, int threads) {
	quickSortParallel(arr.begin(), arr.end(), threads);
}

int main()
{
	// INITIALIZE RANDOM SEED
	srand(time(NULL));
	// Initialize global variables;
	vector<int> array1000;
	vector<int> array10000;
	vector<int> array100000;
	vector<int> array1000000;
	vector<int> array400000;

	array1000 = generateArray(1000, 0, 10000);
	array10000 = generateArray(10000, 0, 10000);
	array100000 = generateArray(100000, 0, 10000);
	array1000000 = generateArray(1000000, 0, 10000);
	array400000 = generateArray(300000, 0, 10000);

	// Serial quick sort times for 1000, 10000, 100000, 1000000
	auto start1 = chrono::high_resolution_clock::now();
	quickSortSerialRun(array1000);
	auto end1 = chrono::high_resolution_clock::now();
	auto timeArraySerial1000 = chrono::duration<double>(end1 - start1).count();
	cout << "Time it took to serially quicksort Array of size: 1000\t\t: " << timeArraySerial1000 << endl;

	auto start2 = chrono::high_resolution_clock::now();
	quickSortSerialRun(array10000);
	auto end2 = chrono::high_resolution_clock::now();
	auto timeArraySerial10000 = chrono::duration<double>(end2 - start2).count();
	cout << "Time it took to serially quicksort Array of size: 10000\t\t: " << timeArraySerial10000 << endl;

	auto start3 = chrono::high_resolution_clock::now();
	quickSortSerialRun(array100000);
	auto end3 = chrono::high_resolution_clock::now();
	auto timeArraySerial100000 = chrono::duration<double>(end3 - start3).count();
	cout << "Time it took to serially quicksort Array of size: 100000\t: " << timeArraySerial100000 << endl;

	auto start4 = chrono::high_resolution_clock::now();
	quickSortSerialRun(array1000000);
	auto end4 = chrono::high_resolution_clock::now();
	auto timeArraySerial1000000 = chrono::duration<double>(end4 - start4).count();
	cout << "Time it took to serially quicksort Array of size: 1000000\t\t: " << timeArraySerial1000000 << endl << "\n\n\n";



	// Parallel QuickSort times for i threads
	for (int i = 1; i <= 16; i*=2)
	{
		auto startp1 = chrono::high_resolution_clock::now();
		quickSortParallelRun(array1000, i*2);
		auto endp1 = chrono::high_resolution_clock::now();
		auto timeArrayParallel1000 = chrono::duration<double>(endp1 - startp1).count();
		cout << "Time it took to quicksort Array of size: 1000 with " << i * 2  << "  Threads: " << timeArrayParallel1000 << endl;

		auto startp2 = chrono::high_resolution_clock::now();
		quickSortParallelRun(array10000, i*2);
		auto endp2 = chrono::high_resolution_clock::now();
		auto timeArrayParallel10000 = chrono::duration<double>(endp2 - startp2).count();
		cout << "Time it took to quicksort Array of size: 10000 with " << i * 2 << "  Threads: " << timeArrayParallel10000 << endl;

		auto startp3 = chrono::high_resolution_clock::now();
		quickSortParallelRun(array100000, i*2);
		auto endp3 = chrono::high_resolution_clock::now();
		auto timeArrayParallel100000 = chrono::duration<double>(endp3 - startp3).count();
		cout << "Time it took to quicksort Array of size: 100000 with " << i * 2<< "  Threads: " << timeArrayParallel100000 << endl;

		auto startp4 = chrono::high_resolution_clock::now();
		quickSortParallelRun(array400000, i);
		auto endp4 = chrono::high_resolution_clock::now();
		auto timeArrayParallel400000 = chrono::duration<double>(endp4 - startp4).count();
		cout << "Time it took to quicksort Array of size: 300000 with " << i * 2 << "  Threads: " << timeArrayParallel400000 << endl <<"\n\n\n";

	}
	return 0;
}

