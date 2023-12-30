#pragma once
#include "_bite_base.h"
#include <vector>
#include <string>
#include <fstream>
#include <memory>


namespace bite
{
	constexpr char16_t F_HexTable[256]
	{
		'00', '10', '20', '30', '40', '50', '60', '70', '80', '90', 'a0', 'b0', 'c0', 'd0', 'e0', 'f0',
		'01', '11', '21', '31', '41', '51', '61', '71', '81', '91', 'a1', 'b1', 'c1', 'd1', 'e1', 'f1',
		'02', '12', '22', '32', '42', '52', '62', '72', '82', '92', 'a2', 'b2', 'c2', 'd2', 'e2', 'f2',
		'03', '13', '23', '33', '43', '53', '63', '73', '83', '93', 'a3', 'b3', 'c3', 'd3', 'e3', 'f3',
		'04', '14', '24', '34', '44', '54', '64', '74', '84', '94', 'a4', 'b4', 'c4', 'd4', 'e4', 'f4',
		'05', '15', '25', '35', '45', '55', '65', '75', '85', '95', 'a5', 'b5', 'c5', 'd5', 'e5', 'f5',
		'06', '16', '26', '36', '46', '56', '66', '76', '86', '96', 'a6', 'b6', 'c6', 'd6', 'e6', 'f6',
		'07', '17', '27', '37', '47', '57', '67', '77', '87', '97', 'a7', 'b7', 'c7', 'd7', 'e7', 'f7',
		'08', '18', '28', '38', '48', '58', '68', '78', '88', '98', 'a8', 'b8', 'c8', 'd8', 'e8', 'f8',
		'09', '19', '29', '39', '49', '59', '69', '79', '89', '99', 'a9', 'b9', 'c9', 'd9', 'e9', 'f9',
		'0a', '1a', '2a', '3a', '4a', '5a', '6a', '7a', '8a', '9a', 'aa', 'ba', 'ca', 'da', 'ea', 'fa',
		'0b', '1b', '2b', '3b', '4b', '5b', '6b', '7b', '8b', '9b', 'ab', 'bb', 'cb', 'db', 'eb', 'fb',
		'0c', '1c', '2c', '3c', '4c', '5c', '6c', '7c', '8c', '9c', 'ac', 'bc', 'cc', 'dc', 'ec', 'fc',
		'0d', '1d', '2d', '3d', '4d', '5d', '6d', '7d', '8d', '9d', 'ad', 'bd', 'cd', 'dd', 'ed', 'fd',
		'0e', '1e', '2e', '3e', '4e', '5e', '6e', '7e', '8e', '9e', 'ae', 'be', 'ce', 'de', 'ee', 'fe',
		'0f', '1f', '2f', '3f', '4f', '5f', '6f', '7f', '8f', '9f', 'af', 'bf', 'cf', 'df', 'ef', 'ff'
	};

	extern std::istream *load_mem_input(byte_t *buf, const size_t sz);
	extern std::ostream *load_mem_output(byte_t *buf, const size_t sz);

	typedef std::unique_ptr<byte_t[]> BufferSmartPtr_t;
	typedef std::shared_ptr<byte_t[]> BufferSharedPtr_t;
	typedef std::vector<BufferSharedPtr_t> BufferVector_t;

	enum class EndianOrder
	{
		Little,
		Big
	};

	template <typename STREAM>
	class StreamFrame
	{
	public:
		using stream_type = STREAM;

		inline StreamFrame()
			: m_stream{ nullptr }, m_order{ EndianOrder::Little }
		{
		}

		inline StreamFrame(std::shared_ptr<STREAM> stream, EndianOrder order)
			: m_stream{ stream }, m_order{ order }
		{
		}

		inline std::shared_ptr<STREAM> getStream()
		{
			return m_stream;
		}

		inline const std::shared_ptr<STREAM> getStream() const
		{
			return m_stream;
		}

		EndianOrder order() const noexcept
		{
			return m_order;
		}

		virtual void move(intptr_t offset) = 0;

		virtual size_t cursor() const = 0;
		virtual size_t size() const = 0;

		inline std::ios::iostate state() const
		{
			return m_stream ? m_stream->rdstate() : -1;
		}

		inline bool valid() const
		{
			return m_stream && m_stream->good();
		}

		// same as valid
		inline operator bool() const
		{
			return valid();
		}

		inline _NODISCARD bool operator!() const
		{
			return !valid();
		}

	protected:
		const EndianOrder m_order;
		std::shared_ptr<STREAM> m_stream;
	};

	class StreamReader : public StreamFrame<std::istream>
	{
	public:
		StreamReader();
		StreamReader(std::shared_ptr<stream_type> stream, EndianOrder order);
		StreamReader(stream_type *stream, EndianOrder order);

		// will copy the buffer
		StreamReader(byte_t* buffer, const size_t buffer_sz, EndianOrder order);
		StreamReader(const std::string &path, EndianOrder order);

		void move(intptr_t offset) override;

		size_t cursor() const override;
		size_t size() const override;

		// [risky to use whith objects]
		// creates a new instance of 'T' and returns it after loading it
		// if ORDER is true then the endian ordering will be apllied
		// when reading integrals/reals 'ORDER' should always be true to make sure accurate readins
		template <typename T, bool ORDER = true>
		T read();

		// [risky to use whith objects]
		// reads without advancing
		// if ORDER is true then the endian ordering will be apllied
		// when reading integrals/reals 'ORDER' should always be true to make sure accurate readins
		template <typename T, bool ORDER = true>
		T peek();

		// 'do_endianeness' can be true if you want an accurate integrale/real value loading
		// but 'do_endianeness' mostly bugs with object loading ( wich you shouldn't do )
		void load(char *buffer, size_t length, bool do_endianeness = false);
		BufferSmartPtr_t read(size_t length);
		BufferSmartPtr_t readCstr(size_t length);
		BufferSmartPtr_t peek(size_t length);
	};

	class StreamWriter : public StreamFrame<std::ostream>
	{
	public:
		StreamWriter();
		StreamWriter(std::shared_ptr<stream_type> stream, EndianOrder order);
		StreamWriter(stream_type *stream, EndianOrder order);
		StreamWriter(const std::string &path, EndianOrder order);

		~StreamWriter();

		void move(intptr_t offset) override;

		size_t cursor() const override;
		size_t size() const override;

		void flush();
		void close();

		// if the second template paramter is true, the current endian order is applyed to make sure integrale/real numbers are written correctly
		template <typename T, bool ORDER = true>
		void write(const T& value);

		void write(const void *buffer, size_t length);

		// will not advance cursor like write()
		void insert(const void *buffer, size_t length);
	};


	extern void M_EndianOrder(char *dst, const char *src, size_t length, EndianOrder order);
	extern BufferSmartPtr_t M_Hexfy(uint8_t *ptr, size_t length);
	// if padding it true, all overflow bytes are set to zero
	extern BufferVector_t M_SplitBuffer(const char *src, size_t src_len, size_t block_len, size_t *last_block_len = nullptr);

	inline const std::string &M_EndianName(const EndianOrder order)
	{
		return std::string(order == EndianOrder::Little ? "little" : "big");
	}

	inline constexpr bool M_EndianNativeToMemory(const EndianOrder order)
	{
#if defined(_WIN32) || defined(__APPLE__) || defined(__MACH__)
		return order == EndianOrder::Little;
#else
		// unix-based
		return order == EndianOrder::Big;
#endif
	}

	inline constexpr EndianOrder M_EndianReverse(const EndianOrder order)
	{
		return (EndianOrder)(((int)order) ^ 1);
	}

	// idk why ORDER is a template.

	template <typename T, bool ORDER>
	inline T StreamReader::read()
	{
		T value{};
		load((char *)&value, sizeof(value), ORDER);
		return value;
	}

	template <typename T, bool ORDER>
	inline T StreamReader::peek()
	{
		T value{};
		load((char *)&value, sizeof(value), ORDER);
		move(-(intptr_t)sizeof(value)); // yeah
		return value;
	}

	template <typename T, bool ORDER>
	inline void StreamWriter::write(const T &value)
	{
		constexpr size_t length{ sizeof(T) };
		if (ORDER)
		{
			if (!M_EndianNativeToMemory(order())) {
				char buffer[ length ];
				M_EndianOrder(buffer, (char *)&value, length, order());
				write(buffer, length);
			}
			else
			{
				write((char *)&value, length);
			}
		}
		else
		{
			write((char *)&value, length);
		}
	}

}
