#include "pch.h"
#include "edited_file.h"

EditedFile::Chunk::Chunk() {
	buffer.resize( ChunkSize, 0 );
}

void EditedFile::set_index( index_t index ) {
	m_file->seekg(index);
}

void EditedFile::move_index( int offset ) {
	set_index( m_chunk_index + offset );
}
