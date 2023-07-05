#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <vector>
#include <CL/cl.hpp>
#include <iostream>
#include "OpenClHelper.h"
#include <chrono>

using namespace std::chrono;
int main()
{
	auto program = createProgram("Kernel.cl");

	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	const unsigned rows = 200;
	const unsigned cols = 300;
	const unsigned long size = rows * cols;
	int array[rows * cols];
	for (int r = 0; r < rows; r++)
		for (int c = 0; c < cols; c++)
			array[r * cols + c]=r;
	std::cout << *array << std::endl;
	cl::Buffer buf(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * size, array);
	cl::Kernel kernel(program, "ProcessTwoDimArray");
	kernel.setArg(0, buf);
	cl::CommandQueue queue(context, device);

	auto start = high_resolution_clock::now();
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(cols, rows));
	queue.enqueueReadBuffer(buf, CL_TRUE, 0, sizeof(int) * size, array);
	auto end = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(end - start);
	std::cout << "Time taken to process a two dimensional " << rows << "x" << cols << " element array using gpu: " << duration.count() << " microseconds" << std::endl;
	return 0;
}