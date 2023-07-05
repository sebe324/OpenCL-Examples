#pragma once

#include <CL/cl.hpp>
#include <fstream>
#include <string>

cl::Program createProgram(const std::string& fileName);