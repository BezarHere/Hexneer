#pragma once
#include <istream>
#include <fstream>
#include "_bite_str.h"
#include "_bite_span.h"

namespace bite
{
	
	template <typename _T, typename _Traits>
	class BasicFile
	{
	public:
		using value_type = _T;
		using traits = _Traits;
		using ifstream = std::basic_ifstream<value_type, traits>;
		using ofstream = std::basic_ofstream<value_type, traits>;
		using string = std::basic_string<value_type, traits>;

		inline static span<value_type> read(const string &path)
		{
			ifstream in{ path };
			const auto _b = in.tellg();
			in.seekg(0, std::ios::end);
			const auto _e = in.tellg();
			in.seekg(0, std::ios::beg);

			const size_t sz = (size_t)(_e - _b);
			span<value_type> s{ sz };
			in.read(s.begin(), sz);
			return s;
		}

	private:
		BasicFile() = delete;
		template <typename _, typename __>
		BasicFile(const BasicFile<_, __> &) = delete;
		template <typename _, typename __>
		BasicFile(BasicFile<_, __> &&) = delete;
		~BasicFile() = delete;
	};


	using File = BasicFile<char, std::char_traits<char>>;
	using WFile = BasicFile<wchar_t, std::char_traits<wchar_t>>;
}