#define CL_USE_DEPRECATED_OPENCL_2_0_APIS


#include <vector>
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <chrono>
#include "OpenClHelper.h"


using namespace std::chrono;

int main()
{
	auto program = createProgram("Kernel.cl");

	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	const long long amount = 1024;
	std::vector<int> arr(amount,1);
	std::vector<int> arr2(amount, 1);
	cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * arr.size(),arr.data());
	cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int)*arr.size());
	cl::Kernel kernel(program,"ProcessArray");
	kernel.setArg(0, inBuf);
	kernel.setArg(1, outBuf);

	cl::CommandQueue queue(context, device);

	auto start = high_resolution_clock::now();
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(arr.size()));
	queue.enqueueReadBuffer(outBuf, CL_FALSE, 0, sizeof(int) * arr.size(), arr.data());

	cl::finish();

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);

	std::cout << "Time taken to process a " << amount << " element array using gpu: " << duration.count() << " microseconds" << std::endl;
	
	start = high_resolution_clock::now();
	for (int i = 0; i < arr2.size(); i++) {
		arr2[i] = arr2[i] * 10;
	}

	stop = high_resolution_clock::now();
	duration = duration_cast<microseconds>(stop - start);
	std::cout << "Time taken to process a " << amount << " element array using cpu: " << duration.count() << " microseconds" << std::endl;
	return 0;
}