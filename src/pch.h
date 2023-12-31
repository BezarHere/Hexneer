#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#define GARTER_STATIC
#include <garter.h>

#if defined(_DEBUG) || !defined(NDEBUG)
#pragma comment(lib, "garter_x64-d")
#pragma comment(lib, "bite64-d")
#else
#pragma comment(lib, "garter_x64")
#pragma comment(lib, "bite64")

#endif // _DEBUG

