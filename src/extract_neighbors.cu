
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "kernels.cuh"

namespace emida {

template<typename T, int s>
__global__ void extract_neighbors(const T* data, const vec2<size_t>* max_i, T* neighbors, size_t cols, size_t rows, size_t batch_size)
{
	int idx = blockIdx.x * blockDim.x + threadIdx.x;

	if (idx >= batch_size)
		return;

	size_t r = (s - 1) / 2;

	auto max = max_i[idx];

	for (int i = 0; i < s; ++i)
	{
		for (int j = 0; j < s; ++j)
		{
			size_t from_x = max.x - r + i;
			size_t from_y = max.y - r + j;

			if (from_x < 0)
				from_x = 0;
			if (from_x >= cols)
				from_x = cols;
			if (from_y < 0)
				from_y = 0;
			if (from_y >= rows)
				from_y = rows;

			neighbors[idx * s * s + j * s + i] = data[idx * cols * rows + from_y * cols + from_x];
		}
	}
}

template<typename T, int s>
void run_extract_neighbors(const T* data, const vec2<size_t>* max_i, T* neighbors, size_t cols, size_t rows, size_t batch_size)
{
	size_t block_size = 128;
	size_t grid_size = div_up(batch_size, block_size);
	extract_neighbors<T, s> << <grid_size, block_size >> > (data, max_i, neighbors, cols, rows, batch_size);
}

template void run_extract_neighbors<double, 3>(const double* data, const vec2<size_t>* max_i, double* neighbors, size_t cols, size_t rows, size_t batch_size);

}