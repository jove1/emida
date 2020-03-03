
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "kernels.cuh"

namespace emida
{

template<typename T>
__global__ void maxarg_reduce(const T* data, data_index<T> * maxes, size_t size)
{
	extern __shared__ data_index<T> sdata[];

	size_t tid = threadIdx.x;
	size_t i = blockIdx.x * blockDim.x + threadIdx.x;
	sdata[tid].data = data[i];
	sdata[tid].index = i;
	__syncthreads();

	for (size_t s = blockDim.x / 2; s > 0; s >>= 1)
	{
		if (tid < s)
		{
			if (sdata[tid + s].data > sdata[tid].data)
			{
				sdata[tid] = sdata[tid + s];
			}
		}
		__syncthreads();
	}
	
	if (tid == 0) maxes[blockIdx.x] = sdata[0];
}

template<typename T, int s>
__global__ void extract_neighbors(const T* data, T* neighbors, size_t max_x, size_t max_y, size_t cols)
{
	int x = blockIdx.x * blockDim.x + threadIdx.x;
	int y = blockIdx.y * blockDim.y + threadIdx.y;
	
	if (x > s || y > s)
		return;

	size_t r = (s - 1) / 2;

	size_t from_x = max_x - r + x;
	size_t from_y = max_y - r + y;

	neighbors[y * s + x] = data[from_y * cols + from_x];
}

template<typename T>
void run_maxarg_reduce(const T* data, data_index<T>* maxes, size_t size, size_t block_size)
{	
	size_t grid_size = div_up(size, block_size);
	maxarg_reduce<T> <<<grid_size, block_size, block_size * sizeof(data_index<T>)>>> (data, maxes, size);
}

template<typename T, int s>
void run_extract_neighbors(const T* data, T* neighbors, size_t max_x, size_t max_y, size_t cols, size_t rows)
{
	dim3 block_size(8, 8);
	dim3 grid_size(div_up(cols, block_size.x), div_up(rows, block_size.y));
	extract_neighbors<T,s> <<<grid_size, block_size>>> (data, neighbors, max_x, max_y, cols);
}


template void run_maxarg_reduce<double>(const double* data, data_index<double>* maxes, size_t size, size_t block_size);

template void run_extract_neighbors<double, 3>(const double* data, double* neighbors, size_t max_x, size_t max_y, size_t cols, size_t rows);

}