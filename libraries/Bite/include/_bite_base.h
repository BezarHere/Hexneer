#pragma once
#include <stdint.h>

namespace bite
{
	typedef uint8_t byte_t;
}

#ifndef _NORETURN
#ifdef __GNUC__
#define _NORETURN __attribute__((noreturn))
#else
#define _NORETURN __declspec(noreturn)
#endif
#endif // _NORETURN

