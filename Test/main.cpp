#define CL_USE_DEPRECATED_OPENCL_2_0_APIS


#include <vector>
#include <CL/cl.hpp>
#include <iostream>

int main()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	std::vector<cl::Device> devices;

	_ASSERT(platforms.size() > 0);
	auto platform = platforms.front();
	platform.getDevices(CL_DEVICE_TYPE_ALL,&devices);

	_ASSERT(devices.size() > 0);
	unsigned int count=1;
	std::cout << "Total number of devices: " << devices.size() << std::endl<<std::endl;
	for (auto device : devices) {

		auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
		auto version = device.getInfo<CL_DEVICE_VERSION >();
		auto local_mem_size = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
		auto smth = device.getInfo<CL_DEVICE_BUILT_IN_KERNELS>();
		std::cout << "Device number " << (count++) << std::endl;
		std::cout << "Version: " << version.c_str() << std::endl;
		std::cout << "Vendor: " << vendor.c_str() << std::endl;
		std::cout << "Local memory size: " << local_mem_size << std::endl;
	}

	return 0;
}