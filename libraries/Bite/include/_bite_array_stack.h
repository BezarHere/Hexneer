#pragma once
#include "_bite_base.h"
#include "_bite_stacklist.h"
#include <stack>
#include <array>

namespace bite
{
	template <typename _T, size_t _SIZE, typename _C = bite::stacklist<_T, _SIZE>>
	class array_stack
	{
	public:
		using value_type = _T;
		using container_type = _C;
		
		inline size_t length() const
		{
			return m_cnt.size();
		}
		
		inline size_t capacity() const
		{
			return m_cnt.capacity();
		}

		inline value_type &top()
		{
			return m_cnt[ m_cnt.size() - 1 ];
		}

		inline const value_type &top() const
		{
			return m_cnt[ m_cnt.size() - 1 ];
		}

		inline void pop()
		{
			return m_cnt.pop_back();
		}

		inline bool push(value_type &&v)
		{
			return m_cnt.push_back(v);
		}

		inline bool push(const value_type &v)
		{
			return m_cnt.push_back(v);
		}

		template <class... _Valty>
		inline bool emplace(_Valty&&... _Val) {
			return m_cnt.emplace_back(std::forward<_Valty>(_Val)...);
		}

		inline bool empty() const
		{
			return m_cnt.empty();
		}

		inline operator bool() const
		{
			return empty();
		}

	private:
		container_type m_cnt;
	};
}
