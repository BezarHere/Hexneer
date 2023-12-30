#pragma once

#include <array>
#include <garter.h>

using gart::Window;

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

constexpr const wchar_t *DefaultTitle = L"Hexneer";

#ifdef UNICODE
using filepath = std::wstring;
#else
using filepath = std::string;
#endif // UNICODE

