#pragma once
#include <string>
#include <vector>
#include "_bite_span.h"


#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#define BITE_FORCEINLINE
#endif

#define IS_UPPER(n) n >= 'A' && n <= 'Z'
#define IS_LOWER(n) n >= 'a' && n <= 'z'


namespace std
{
	inline string to_string(const char *_Ptr)
	{
		return string(_Ptr);
	}
}

namespace bite
{
	constexpr char Lower = 'a' - 'A';
	constexpr char Upper = 'A' - 'a';

	template <typename _Elm>
	class basic_string_view
	{
	public:
		using string_type = std::basic_string<_Elm>;
		using ptr_type = _Elm *;
		using const_ptr_type = const _Elm *;
		using size_type = typename string_type::size_type;

		inline basic_string_view(const string_type &str)
			: m_buf{ str.c_str() }, m_sz{ str.length() }
		{
		}

		inline basic_string_view(const_ptr_type cstr, const size_type sz)
			: m_buf{ cstr }, m_sz{ sz }
		{
		}

		inline basic_string_view(const_ptr_type cstr)
			: basic_string_view(cstr, strlen(cstr))
		{
		}


		inline size_type length() const
		{
			return m_sz;
		}

		inline size_type size() const
		{
			return m_sz;
		}

		inline const_ptr_type c_str()
		{
			return m_buf;
		}

		inline const const_ptr_type c_str() const
		{
			return m_buf;
		}

		inline const_ptr_type data()
		{
			return m_buf;
		}

		inline const const_ptr_type data() const
		{
			return m_buf;
		}

		inline const_ptr_type begin()
		{
			return m_buf;
		}

		inline const const_ptr_type begin() const
		{
			return m_buf;
		}

		inline const_ptr_type end()
		{
			return m_buf + m_sz;
		}

		inline const const_ptr_type end() const
		{
			return m_buf + m_sz;
		}

	private:
		const const_ptr_type m_buf;
		const size_type m_sz; // including terminating zero
	};

	using string_view = basic_string_view<char>;
	using wstring_view = basic_string_view<wchar_t>;


	typedef uint8_t byte_t;

	// modifies the dst to be a copy of src lowered(uncapitalized) from the start to 'len'
	inline void strlower(char *dst, const char *const src, const size_t len)
	{
		for (size_t i{}; i < len; i++)
		{
			if (IS_UPPER(src[ i ])) dst[ i ] = src[ i ] + Lower;
		}
	}

	// modifies the dst to be a copy of src uppered(capitalized) from the start to 'len'
	inline void strupper(char *dst, const char *const src, const size_t len)
	{
		for (size_t i{}; i < len; i++)
		{
			if (IS_LOWER(src[ i ])) dst[ i ] = src[ i ] + Upper;
		}
	}


	inline std::string strlower(const std::string &str)
	{
		char *f = new char[ str.size() + 1 ];
		strlower(f, (const char *)str.c_str(), str.size());
		f[ str.size() ] = '\0';
		return std::move(std::string(f));
	}

	inline std::string strupper(const std::string &str)
	{
		char *f = new char[ str.size() + 1 ];
		strupper(f, (const char *)str.c_str(), str.size());
		f[ str.size() ] = '\0';
		return std::move(std::string(f));
	}

	// stops at the first null char in 'src'
	// modifies the dst to be a copy of src lowered(uncapitalized) from the start
	// to the index of the first null char in src
	inline void strlower(char *dst, const char *const src)
	{
		size_t i{};
		do
		{
			if (!src[ i ]) return;
			if (IS_UPPER(src[ i ])) dst[ i ] = src[ i ] + Lower;
		} while (++i);
	}

	// stops at the first null char in 'src'
	// modifies the dst to be a copy of src uppered(capitalized) from the start
	// to the index of the first null char in src
	inline void strupper(char *dst, const char *const src)
	{
		size_t i{};
		do
		{
			if (!src[ i ]) return;
			if (IS_LOWER(src[ i ])) dst[ i ] = src[ i ] + Upper;
		} while (++i);
	}

	// modifies the cstring from the start to len to be upper
	inline void strupper(char *cstr, size_t len)
	{
		strupper(cstr, cstr, len);
	}

	// modifies the cstring from the start to len to be lower
	inline void strlower(char *cstr, size_t len)
	{
		strlower(cstr, cstr, len);
	}

	// modifies the cstring to be upper
	inline void strupper(char *cstr)
	{
		strupper(cstr, cstr);
	}

	// modifies the cstring to be lower
	inline void strlower(char *cstr)
	{
		strlower(cstr, cstr);
	}

	// retruns: a cstring copy of src to upper with size len + 1
	//	as the last additional char is a null char
	inline char *strupper(const char *const src, size_t len)
	{
		char *temp = new char[ len + 1 ];
		strupper(temp, src, len);
		temp[ len ] = 0;
		return temp;
	}

	// retruns: a cstring copy of src to lower with size len + 1
	//	as the last additional char is a null char
	inline char *strlower(const char *const src, size_t len)
	{
		char *temp = new char[ len + 1 ];
		strlower(temp, src, len);
		temp[ len ] = 0;
		return temp;
	}

	// stops at the first null char
	// retruns: a cstring copy of src to upper
	inline char *strupper(const char *const src)
	{
		// -1 to remove the added null char/len++ at the other strupper()
		const size_t len = sizeof(src) - 1;
		return strupper(src, len);
	}

	// stops at the first null char
	// retruns: a cstring copy of src to lower
	inline char *strlower(const char *const src)
	{
		// -1 to remove the added null char/len++ at the other strupper()
		const size_t len = sizeof(src) - 1;
		return strlower(src, len);
	}

	// returns a substr *copy* from index to len, doesn't modify the original str
	inline char *strslice(const std::string &src, const size_t index, const size_t len)
	{
		return (char *)memcpy(new char[len + 1] {}, src.c_str() + index, len);
	}

	inline char *strslice(const char *const src, const size_t index, const size_t len)
	{
		return (char *)memcpy(new char[len + 1] {}, src + index, len);
	}

	inline size_t strcount(const std::string &str, const char c, const size_t start = 0, size_t end = 0)
	{
		if (end <= start)
			end = str.length();
		size_t n{};
		for (size_t i{ start }; i < end; i++)
			if (str[ i ] == c) n++;
		return n;
	}

	inline size_t strcount(const char *const cstr, const char c, const size_t len)
	{
		size_t n{};
		for (size_t i{}; i < len; i++)
			if (cstr[ i ] == c) n++;
		return n;
	}

	inline std::string format(const std::string &str, const std::string &sub)
	{
		const char *cstr = str.c_str();
		const size_t n = str.length();

		const char *sub_cstr = sub.c_str();
		const size_t sub_n = sub.length();

		std::vector<size_t> replace_anchors{};

		for (size_t i = 0; i < n - 1; i++)
		{
			// skip escaped chars
			if (cstr[ i ] == '\\')
			{
				i++;
				continue;
			}

			if (cstr[ i ] == '{' && cstr[ i + 1 ] == '}')
				replace_anchors.push_back(i++);
		}

		// no anchor to format
		if (!replace_anchors.size())
			return str;

		const size_t final_n = n + ((-2 + sub_n) * replace_anchors.size());
		span<char> result_cstr{ final_n };


		size_t write_offset = 0;
		size_t read_offset = 0;
		size_t replacment_str_index = 0;

		for (size_t i = 0; i < n - replace_anchors.size(); i++)
		{
			// this index is the position of a repacment string
			if (i + read_offset == replace_anchors[ replacment_str_index ])
			{

				for (size_t j = 0; j < sub_n; j++)
				{
					result_cstr[ write_offset++ ] = sub_cstr[ j ];
				}

				read_offset++;
				// now processing the next replacment
				replacment_str_index++;
			}
			else
				// no replacment str, fill with the base str chars
				result_cstr[ write_offset++ ] = str[ i + read_offset ];
		}

		return std::string{ result_cstr.data().get(), result_cstr.size() };
	}

	inline std::string formatv(const std::string &str, std::initializer_list<std::string> s)
	{
		const char *cstr = str.c_str();
		const size_t n = str.length();
		const size_t sz = s.size();
		size_t c_sn = 0, replace_tn = 0;
		span<size_t> replacment_str_pos{ sz };

		for (size_t i = 0; i < n - 1; i++)
		{
			// skip escaped chars
			if (cstr[ i ] == '\\')
			{
				i++;
				continue;
			}

			if (cstr[ i ] == '{' && cstr[ i + 1 ] == '}')
			{
				replace_tn += s.begin()[ c_sn ].length();
				replacment_str_pos[ c_sn++ ] = i++;
				if (c_sn >= sz)
					break;
			}
		}

		const size_t final_n = n - (c_sn * 2) + replace_tn;
		span<char> result_cstr{ final_n };

		size_t write_offset = 0;
		size_t read_offset = 0;
		size_t replacment_str_index = 0;

		for (size_t i = 0; i < n - c_sn; i++)
		{
			// this index is the position of a repacment string
			if (i + read_offset == replacment_str_pos[ replacment_str_index ])
			{
				const size_t rs_len = s.begin()[ replacment_str_index ].length();
				const char *const res_cstr = s.begin()[ replacment_str_index ].c_str();

				for (size_t j = 0; j < rs_len; j++)
				{
					result_cstr[ write_offset++ ] = res_cstr[ j ];
				}

				read_offset++;
				// now processing the next replacment
				replacment_str_index++;
			}
			else
				// no replacment str, fill with the base str chars
				result_cstr[ write_offset++ ] = str[ i + read_offset ];
		}

		return std::string{ result_cstr.data().get(), result_cstr.size() };
	}

	template <typename _T>
	inline void _format_unpack_stringfy(std::string *s, const _T &t)
	{
		(*s) = std::to_string(t);
	}

	template <typename _T, typename ..._VT>
	inline void _format_unpack_stringfy(std::string *s, const _T &t, const _VT &... v)
	{
		(*s++) = std::to_string(t);
		_format_unpack_stringfy(s, v...);
	}

	template <typename ..._Vals>
	inline std::string format(const std::string &str, const _Vals &... vals)
	{
		constexpr size_t ArgC = sizeof...(vals);
		std::string strs[ ArgC ];
		const std::string *pstr = strs;
		size_t i = 0;

		_format_unpack_stringfy(strs + i++, vals...);
		return formatv(str, std::initializer_list<std::string>(pstr, pstr + ArgC));
	}


}

#ifdef BITE_FORCEINLINE
#undef FORCEINLINE
#undef BITE_FORCEINLINE
#endif
#undef IS_LOWER
#undef IS_UPPER