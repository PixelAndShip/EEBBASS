#pragma once

#include <fstream>
#include <iostream>
#include <string>

extern std::ofstream debugFile;

#define DEBUG_LOG(x) \
    debugFile << x << std::endl;
