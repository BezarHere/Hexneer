#pragma once
#include "_bite_base.h"


// VERY BAD!!!!!!
#ifdef min
//#error Illagle 'min' macro
#define MIN min
#undef min
#endif // min
#ifdef max
//#error Illagle 'max' macro
#define MAX max
#undef max
#endif // max


namespace bite
{
	struct range
	{
		inline constexpr range()
			: min{}, max{}
		{
		}

		inline constexpr range(size_t min_v, size_t max_v)
			: min{ min_v }, max{ max_v }
		{
		}

		template <typename _T, typename _E>
		inline constexpr range(const std::pair<_T, _E> &pair)
			: min{ pair.first }, max{ pair.second }
		{
		}


		inline size_t size() const noexcept
		{
			return max - min;
		}

		inline operator bool() const noexcept
		{
			return max >= min;
		}

		inline bool operator!() const noexcept
		{
			return max < min;
		}

		template <typename _T, typename _E>
		inline operator std::pair<_T, _E>()
		{
			return { min, max };
		}

		inline range &expand(size_t v)
		{
			min -= v;
			max += v;
			return *this;
		}

		size_t min, max;
	};
}

