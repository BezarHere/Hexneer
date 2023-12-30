#pragma once
#include "_bite_base.h"
#include <type_traits>

namespace bite
{
	template <typename _T>
	class stackptr
	{
	public:
		using value_type = _T;
		using buffer_type = int;
		static constexpr size_t buffer_size = sizeof( value_type ) / sizeof( buffer_type ) + ( sizeof value_type % sizeof buffer_type != 0 );

		// will not allocate the ptr
		inline stackptr(std::nullptr_t)
			: m_ptr{ nullptr } {
		}

		// will alocate the ptr with default constructor
		inline stackptr()
			: m_ptr{ allocate() } {
		}

		template <typename... _Valty>
		inline stackptr( const _Valty &... args ) {
			allocate( args... );
		}

		inline stackptr( const stackptr &copy ) {
			allocate( *copy.m_ptr );
		}

		inline stackptr( stackptr &&move ) {
			/// FIXME: is this safe?
			allocate( std::move( *move.m_ptr ) );
			move.m_ptr = NULL;
		}

		inline ~stackptr() {
			deallocate();
		}

		template <typename... _Valty>
		inline value_type *allocate( _Valty &&... args ) {
			return m_ptr = new(m_buf) value_type( args... );
		}

		// nullptr safe
		inline void deallocate() noexcept {
			if (!m_ptr) return;
			m_ptr->~value_type();
			m_ptr = nullptr;
		}

		inline value_type *operator->() {
			return m_ptr;
		}

		inline const value_type *operator->() const {
			return m_ptr;
		}

		inline value_type &operator*() {
			return *m_ptr;
		}

		inline const value_type &operator*() const {
			return *m_ptr;
		}

		inline value_type *get_ptr() {
			return m_ptr;
		}

		inline const value_type *get_ptr() const {
			return m_ptr;
		}

		inline operator bool() const noexcept {
			return m_ptr;
		}

	private:
		buffer_type m_buf[ buffer_size ] = { 0 };
		value_type *m_ptr = nullptr;
	};

}
