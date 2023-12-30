#pragma once

#include <array>
#include <garter.h>
#include <bite.h>

using gart::Window;
using bite::stacklist;
using bite::stackptr;

typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;

typedef size_t index_t;

struct Version
{
	u8 major, minor;
	u16 patch;

	inline constexpr i32 repr() const noexcept {
		return i32(major << 24) + (minor << 16) + patch;
	}

};
static_assert(sizeof( Version ) == sizeof( i32 ), "Version should be the same size as an int");

constexpr const wchar_t *DefaultTitle = L"Hexneer";
constexpr Version AppVersion = { 0, 2, 0 };
constexpr i32 AppVersionId = AppVersion.repr();

#ifdef UNICODE
using filepath = std::wstring;
#else
using filepath = std::string;
#endif // UNICODE

#define KILOBYTE(x) ((x) << 10)
#define MEGABYTE(x) ((x) << 20)
#define GIGABYTE(x) ((x) << 30)
