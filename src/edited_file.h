#pragma once
#include "gh.h"



class EditedFile
{
public:
	static constexpr size_t ChunkSize = MEGABYTE(8);
	struct Chunk
	{
		Chunk();
		std::vector<u8> buffer;
	};

	void set_index( index_t index );
	void move_index(int offset);

	inline index_t get_index() const noexcept {
		return m_chunk_index;
	}

private:
	Chunk m_current_chunk;
	index_t m_chunk_index;
	filepath m_filepath;
	std::ifstream *m_file;
};
