#pragma once
#include "_bite_base.h"
#include <array>
#include <exception>

#ifndef IF_CONSTEXPR
#if _HAS_CXX17
#define IF_CONSTEXPR if constexpr
#else
#define IF_CONSTEXPR if
#endif
#endif

namespace bite
{
	struct test
	{
		int x, y, z;
		std::array<float, 4> more4;
	};

	template <typename _T, size_t _SZ, bool _NOTHROW = false>
	class MemoryPool
	{
	public:
		using this_type = MemoryPool<_T, _SZ>;
		using element_type = _T;
		using ab_int_type = uint8_t;
		using size_type = size_t;

		static constexpr size_type NPos = static_cast<size_type>(-1);

		static constexpr size_type ArraySize = _SZ;
		static constexpr size_type ArrayPoolSize = _SZ * sizeof(element_type);
		static constexpr size_type AllocMapIntBits = sizeof( ab_int_type ) * 8;
		static constexpr size_type AllocMapIntLSBits = 3; // 1 << AllocMapIntLSBits == AllocMapIntBits
		static constexpr size_type AllocMapShiftmask = AllocMapIntBits - 1;
		static constexpr size_type AllocMapSize = (ArraySize / AllocMapIntBits) + 1;

		struct dummy_element
		{
			uint8_t _[ sizeof( element_type ) ];
		};

		using container_type = std::array<dummy_element, ArraySize>;
		using alloc_bitmap_type = std::array<ab_int_type, AllocMapSize>;

		static constexpr bool NoThrow = _NOTHROW;

		static_assert(1 << AllocMapIntLSBits == AllocMapIntBits, "'1 << AllocMapIntLSBits == AllocMapIntBits' should be always true");

		~MemoryPool()
		{
			
		}

		template <typename ..._Valty>
		inline element_type *allocate( _Valty&&... args )
		{
			const size_type slot = find_slot();
			if (slot == NPos)
			{
				
				IF_CONSTEXPR(NoThrow)
					return nullptr;
				std::_Xbad_alloc();
			}

			m_allocmap[ slot >> AllocMapIntLSBits ] |= 1 << (slot & AllocMapShiftmask);
			return new (reinterpret_cast<void *>(m_pool.data() + slot)) element_type( std::forward<_Valty>(args)... );
		}

		inline void free( const element_type *ptr )
		{
			if (!is_allocated( ptr ))
			{
				IF_CONSTEXPR( NoThrow )
					return;
				std::_Xinvalid_argument( "ptr" ); // <- freeing an element more then once
			}
			
			ptr->~element_type();
			const size_t slot = get_slot( ptr );
			m_allocmap[ slot >> AllocMapIntLSBits ] &= ~(1 << (slot & AllocMapShiftmask));
		}

		inline bool is_allocated( const element_type *ptr ) const noexcept
		{
			if (!inrange( ptr ))
				return false;
			return slot_used( static_cast<size_type>(ptr - reinterpret_cast<const element_type *>(m_pool.data())) );
		}

		inline bool slot_used( const size_type index ) const noexcept
		{
			return m_allocmap[ index >> AllocMapIntLSBits ] >> (index & AllocMapShiftmask);
		}

		inline size_type find_slot() const noexcept
		{
			for (size_type i = 0; i < AllocMapSize; i++)
			{
				const ab_int_type allocsect = ~m_allocmap[ i ];
				if (!allocsect)
					continue;

				// TODO: improve
				for (size_type j = 0; j < AllocMapIntBits; j++)
				{
					if (allocsect & (1ull << j))
						return (i << AllocMapIntLSBits) + j;
				}
			}
			return NPos;
		}

		inline bool inrange( const element_type *ptr ) const
		{
			return ptr >= reinterpret_cast<const element_type *>(m_pool.data()) && ptr - reinterpret_cast<const element_type *>(m_pool.data()) < ArraySize;
		}

		inline size_t get_slot( const element_type *ptr )
		{
			return ptr - reinterpret_cast<const element_type *>(m_pool.data());
		}

	private:
		alloc_bitmap_type m_allocmap;
		container_type m_pool;
	};


}
