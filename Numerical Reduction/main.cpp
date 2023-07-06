#define CL_USE_DEPRECATED_OPENCL_2_0_APIS


#include <vector>
#include <CL/cl.hpp>
#include <iostream>
#include "OpenClHelper.h"
#include <numeric>

int main()
{

	cl::Program program = createProgram("NumericalReduction.cl");
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto device = devices.front();

	std::vector<int> vec(2048);
	long count = 0;
	for (int i = 0; i < vec.size(); i++){
		vec[i] = i;
		count += i;
		}
	std::cout << count << std::endl;
	cl::Kernel kernel(program, "NumericalReduction");
	auto workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);
	auto numWorkGroups = vec.size() / workGroupSize;
	cl::Buffer inBuf(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, vec.size() * sizeof(int), vec.data());
	cl::Buffer outBuf(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, numWorkGroups*sizeof(int));
	
	kernel.setArg(0, inBuf);
	kernel.setArg(1, sizeof(int) * workGroupSize, nullptr);
	kernel.setArg(2, outBuf);


	std::vector<int> outVec(numWorkGroups);

	cl::CommandQueue queue(context, device);

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(workGroupSize*numWorkGroups));
	queue.enqueueReadBuffer(outBuf, GL_TRUE, 0, sizeof(int) * outVec.size(), outVec.data());
	
	std::cout << std::accumulate(outVec.cbegin(), outVec.cend(), 0) << std::endl;
	
	std::cin.get();
	return 0;
}