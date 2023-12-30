#pragma once
#include <string>
#include "_bite_dye.h"

namespace bite
{
	// when logicly, the code can't normaly reach a given branch
	// for examble:
	// int foo(int x)
	// {
	//	if (!x)
	//		return ~x;
	//	for (int i = 0; i < 32; i++)
	//		{
	//			if ((1 << i) & x) return i;
	//		}
	//		// possibly another process screwing the memory or sm idk
	//		throw impossible_branch("how can x not equal zero and the same time have no bits set to 1");
	//	}
	class impossible_branch : public std::exception
	{
	public:
		using base_class = std::exception;
		impossible_branch() noexcept
			: base_class()
		{
		}

		explicit impossible_branch(char const *const _Message) noexcept
			: base_class(_Message)
		{
		}

		impossible_branch(char const *const _Message, int code) noexcept
			: base_class(_Message, code)
		{
		}
	};

	 inline void raise(const std::exception &exc)
	{
		dye::dye(exc, { dye::ColorCode::LightRed, dye::ColorCode::Black }, std::cerr);
		throw exc;
	}

	_NORETURN inline void raise(const std::string &msg)
	{
		raise(std::runtime_error(msg));
	}

	_NORETURN inline void raise(const char *const msg)
	{
		raise(std::runtime_error((!msg || !*msg) ? "Error" : msg));
	}

	inline void warn(const char *const msg)
	{
		if (!msg || !*msg)
			return warn("Warning!");
		dye::dye(msg, { dye::ColorCode::LightYellow, dye::ColorCode::Black }, std::cerr);
		std::cerr << '\n';
	}

	inline void warn(const std::string &msg)
	{
		warn(msg.c_str());
	}
}
