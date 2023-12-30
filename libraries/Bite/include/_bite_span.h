#pragma once
#include "_bite_base.h"
#include "_bite_range.h"
#include <utility>



namespace bite
{

	// takes ownership of pointers
	template <typename _T>
	class span
	{
	public:
		using value_type = _T;
		using reference_type = _T &;
		using ptr_type = value_type *;
		using const_ptr_type = const value_type *;
		using smart_ptr_type = std::shared_ptr<value_type[]>;
		using cptr_type = const _T *;
		using pair_type = std::pair<smart_ptr_type, size_t>;
		using this_type = span<value_type>;

		inline static constexpr pair_type copy_range(const_ptr_type const begin, const size_t size)
		{
			if (!size) return { smart_ptr_type(nullptr), size };
			ptr_type p = (ptr_type)memcpy((void *)new value_type[ size ], begin, sizeof(value_type) * size);
			return { smart_ptr_type(p), size };
		}

		inline static constexpr void copy_range_to(const_ptr_type const begin, const size_t size, pair_type &pair)
		{
			pair.second = size;
			if (!size)
			{
				pair.first.reset(nullptr);
				return;
			}
			ptr_type p = (ptr_type)memcpy((void *)new value_type[ size ], begin, sizeof(value_type) * size);
			pair.first.reset(p);
		}

		inline span() : m_vals{ nullptr, 0 } {  }
		inline span(ptr_type begin, ptr_type end) : m_vals{ copy_range(begin, end - begin) } { _VerfyRange(); }

		inline explicit span(ptr_type begin, size_t end) : m_vals{ copy_range(begin, end) } {}
		inline explicit span(smart_ptr_type ptr, size_t size) : m_vals{ ptr, size } {}
		inline explicit span(size_t size) : span(new value_type[ size ]{}, size) {}

		inline span(const this_type &copy)
			: m_vals{ copy.m_vals }
		{
			_VerfyRange();
		}

		inline span(this_type &&move) noexcept
			: m_vals{ move.m_vals }
		{
		}

		// clear memory
		inline ~span()
		{
		}

		inline void swap(span<_T> &other) noexcept
		{
			if (this == std::addressof(other))
				return;

			// swaping ranges will be enough
			m_vals.first.swap(other.m_vals.first);
			const size_t end = m_vals.second;
			m_vals.second = other.m_vals.second;
			other.m_vals.second = end;
		}

		inline this_type &operator=(const this_type &copy)
		{
			copy._VerfyRange();
			m_vals = copy.m_vals;
			return *this;
		}

		inline this_type &operator=(this_type &&move) noexcept
		{
			m_vals = move.m_vals;
			return *this;
		}

		inline bool operator==(const this_type &eq)
		{
			return eq.begin() == begin() && eq.end() == end();
		}

		inline ptr_type begin() noexcept {
			return m_vals.first.get();
		}

		inline cptr_type begin() const noexcept {
			return m_vals.first.get();
		}

		inline ptr_type end() noexcept {
			return m_vals.first.get() + m_vals.second;
		}

		inline cptr_type end() const noexcept {
			return m_vals.first.get() + m_vals.second;
		}

		inline size_t size() const noexcept
		{
			return m_vals.second;
		}

		inline bool empty() const noexcept
		{
			return !m_vals.first || !m_vals.second;
		}

		inline smart_ptr_type &data() noexcept
		{
			return m_vals.first;
		}

		inline const smart_ptr_type &data() const noexcept
		{
			return m_vals.first;
		}

		inline reference_type operator[](const size_t index)
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			if (index >= size())
				throw std::exception("Out-of-range index");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return m_vals.first.get()[ index ];
		}

		inline const reference_type operator[](const size_t index) const
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			if (index >= size())
				throw std::exception("Out-of-range index");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			return m_vals.first.get()[ index ];
		}

		inline const this_type &operator[](const range slice_range) const
		{
#if _CONTAINER_DEBUG_LEVEL > 0
			if (slice_range.max >= size() || !slice_range)
				throw std::exception("Out-of-range index");
#endif // _CONTAINER_DEBUG_LEVEL > 0
			if (!slice_range.size())
			{
				return this_type();
			}
			// automaticly copied
			return this_type(m_vals.first.get() + slice_range.min, slice_range.size());
		}

		inline operator bool() const noexcept
		{
			return m_vals.first && m_vals.second;
		}


	private:
		inline void _VerfyRange() const
		{
			// the valid range states are Zero-Length nullptr or Fixed-Length ptr
			if (!m_vals.first != !m_vals.second)
				throw std::exception("Invalid span ends");
		}

		inline void _Release() noexcept
		{
			m_vals.first = nullptr;
			m_vals.second = 0;
		}

	private:
		pair_type m_vals;
	};

}
