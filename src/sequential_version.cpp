#include <vector>
#include <random>
#include <ctime>
#include <cassert>
#include <iostream>
#include <omp.h>

template <typename T>
void insert_sort(std::vector<T> &arr, int i, int half) {
	int j = 0;
	for (int k = half + i; k < arr.size(); k = k + half)
	{
		j = k;
		while (j > i && arr[j - half] > arr[j])
		{
			std::swap(arr[j], arr[j - half]);
			j = j - half;
		}
	}
}

template <typename T>
std::vector<T> shell_sort_parallel(std::vector<T> arr)
{
	assert(arr.size() > 0);
	auto end = arr.size() - 1;

	unsigned h = end / 2;
	for (h = 1; h <= end / 9; h = 3 * h + 1)
		;
	int i;
	int size = arr.size();
	for (; h > 0; h /= 3)
	{
        #pragma omp parallel for shared(arr, size, h) private (i) default(none)
		for (i = 0; i < h; i++)
		{
			InsertSort(arr, i, h);
		}
	}
	return arr;
}

template <typename T>
std::vector<T> shell_sort(std::vector<T> arr)
{
	assert(arr.size() > 0);
	auto end = arr.size() - 1;

	unsigned h = end / 2;
	for (h = 1; h <= end / 9; h = 3 * h + 1)
		;
	for (; h > 0; h /= 3)
		for (auto i = h; i <= end; i++)
		{
			unsigned j = i;
			T v = arr[i];
			while (j >= h && v < arr[j - h])
			{
				arr[j] = arr[j - h];
				j -= h;
			}
			arr[j] = v;
		}
	return arr;
}

int main() {
	constexpr auto test_size = 100000;
	std::mt19937 gen(time(0));
	std::uniform_int_distribution<> random(0, 1000000);
	std::vector<int> unsorted_array;


	for (auto i = 0; i < test_size; ++i)
		unsorted_array.emplace_back(random(gen));



	std::cout << std::endl;
	float fTimeStart = clock() / (float)CLOCKS_PER_SEC;
	auto sorted_array = shell_sort(unsorted_array);
	float fTimeStop = clock() / (float)CLOCKS_PER_SEC;

	std::cout << "sequential_version time : " << fTimeStop - fTimeStart << std::endl;

	std::cout << std::endl;
	fTimeStart = clock() / (float)CLOCKS_PER_SEC;
	auto parallel_sorted_array = shell_sort_parallel(unsorted_array);
	fTimeStop = clock() / (float)CLOCKS_PER_SEC;
	std::cout << "parallel_version time : " << fTimeStop - fTimeStart << std::endl;
	/*
	for (auto&& elem : sorted_array)
		std::cout << elem << " ";
	std::cout << std::endl;
	for (auto&& elem : parallel_sorted_array)
		std::cout << elem << " ";
		*/
}