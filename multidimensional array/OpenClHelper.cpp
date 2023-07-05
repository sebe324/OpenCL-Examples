#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include "OpenClHelper.h"

cl::Program createProgram(const std::string& fileName) {
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	auto platform = platforms.front();


	std::vector<cl::Device> devices;

	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);

	auto device = devices.front();

	std::ifstream file(fileName);

	std::string src(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	cl::Context context(device);

	cl::Program program(context, sources);

	program.build();

	return program;
}