#include "firstlight.h"
#include "stream/BufferStream.h"

namespace flt
{

//////////////////////////////////////////////////////////////////////////

void BufferStream::ReserveBuffer(int newCapacity)
{
	if(newCapacity > m_capacity)
	{
		const int mask = ~(BUFFER_INITIAL_SIZE-1);

		int actualCapacity = (newCapacity + (~mask)) & mask;

		if(actualCapacity < BUFFER_INITIAL_SIZE )
			actualCapacity = BUFFER_INITIAL_SIZE;

		char* buffer = new char[actualCapacity];
		
		if(m_size > 0)
			memcpy(buffer, m_buffer, m_size );
		
		if(m_defaultBuffer != m_buffer)
			delete [] m_buffer;

		m_buffer = buffer;
		m_capacity = actualCapacity;
	}
}

//////////////////////////////////////////////////////////////////////////

BufferStream::BufferStream() :
	Stream(),
	m_openMode(0),
	m_buffer(0),
	m_size(0),
	m_capacity(0),
	m_offset(0),
	m_ownsBuffer(false)
{

}

//////////////////////////////////////////////////////////////////////////

BufferStream::BufferStream(int openMode, int size, void* buffer, bool takeOwnerShip) :
	Stream(),
	m_buffer(0),
	m_capacity(0),
	m_offset(0),
	m_ownsBuffer(true)
{
	Open(openMode, size, buffer, takeOwnerShip);
}

//////////////////////////////////////////////////////////////////////////

BufferStream::BufferStream(int openMode, int size, void* buffer) :
	Stream(),
	m_buffer(0),
	m_capacity(0),
	m_offset(0),
	m_ownsBuffer(true)
{
	Open(openMode, size, buffer);
}

//////////////////////////////////////////////////////////////////////////

BufferStream::~BufferStream()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////

void BufferStream::Open(int openMode, int size, void* buffer, bool takeOwnerShip)
{
	Open(openMode, size, buffer);

	m_ownsBuffer = takeOwnerShip;
}

//////////////////////////////////////////////////////////////////////////

void BufferStream::Open(int openMode, int size, void* buffer)
{
	Close();

	m_openMode	= openMode;
	m_offset	= 0;
	m_size		= 0;

	if(buffer == 0)
	{
		//FLT_ASSERT(m_openMode & io::out);

		m_ownsBuffer= true;
		m_buffer	= m_defaultBuffer;
		m_capacity	= sizeof(m_defaultBuffer);

		ReserveBuffer(size);

		m_size = size;
	}
	else
	{
		m_buffer = (char*)buffer;
		m_size = m_capacity = size;
		m_ownsBuffer = false;
	}

	m_isOpen = true;
}

//////////////////////////////////////////////////////////////////////////

void BufferStream::Close()
{
	if(m_ownsBuffer)
	{
		if(m_buffer != m_defaultBuffer && m_buffer != NULL)
		{
			delete [] m_buffer;
			m_buffer = NULL;
		}

		m_buffer = m_defaultBuffer;
		m_size = 0;
		m_capacity = sizeof(m_defaultBuffer);
	}
/*	
	else if(m_isOpen)
	{
		int i = 0;
	}
*/
	m_isOpen = false;
}

//////////////////////////////////////////////////////////////////////////

int BufferStream::Read(void* buffer, int size)
{
	FLT_ASSERT(m_openMode & io::in);

	if(m_offset + size > m_size)
	{
		size = m_size - m_offset;
	}

	memcpy(buffer, m_buffer + m_offset, size);
	
	m_offset += size;

	return size;
}

//////////////////////////////////////////////////////////////////////////

int BufferStream::Write(const void* buffer, int size)
{
	FLT_ASSERT(m_openMode & io::out);

	const int newSize = m_offset + size;

	if(newSize > m_size)
	{
		if(newSize <= m_capacity)
			m_size = m_offset + size;
		else
		{
			if(m_ownsBuffer)
				ReserveBuffer(newSize);
			else
				FLT_ASSERT(false);

			m_size = m_offset + size;

			FLT_ASSERT(m_size <= m_capacity);
		}
	}

	memcpy(m_buffer + m_offset, buffer, size);

	m_offset += size;

	return size;
}

//////////////////////////////////////////////////////////////////////////

int BufferStream::GetTotalSize(void)
{
	// Save current position
	const int oldOffset = m_offset;

	// Get size
	Seek(0, io::end);
	const int fileSize = Tell();

	// Restore stream position
	Seek(oldOffset, io::beg);

	return fileSize;
}

//////////////////////////////////////////////////////////////////////////

void BufferStream::Seek(int offset, int origin)
{
	int newOffset = m_offset;
	if(origin == io::beg)
		newOffset = offset;
	else if(origin == io::cur)
		newOffset += offset;
	else if(origin == io::end)
		newOffset = m_size + offset;
	else
		FLT_ASSERT(false);
	
	if(newOffset < 0)
	{
		FLT_ASSERT(false); //should I assert here ??
		m_offset = 0;
	}
	else if(newOffset > m_size)
	{
		if(m_ownsBuffer && (m_openMode & io::out)) //grow the buffer only if io::out flag is set and we own buffer
		{
			if(newOffset > m_capacity)
				ReserveBuffer(newOffset);
			
			m_offset = m_size;
		}
		else
		{
			FLT_ASSERT(false);
		}
	}
	else
	{
		m_offset = newOffset;
	}
}

//////////////////////////////////////////////////////////////////////////

void* BufferStream::GetBytes(int size)
{
	FLT_ASSERT(size >= 0);
	//FLT_ASSERT(m_offset + size < m_size);

	Seek(size, io::cur);

	return m_buffer + m_offset - size;
}

//////////////////////////////////////////////////////////////////////////

void BufferStream::Write(Stream& stream, int size)
{
	FLT_ASSERT(m_openMode & io::out);

	const int newSize = m_offset + size;

	if(newSize > m_size)
	{
		if(newSize <= m_capacity)
			m_size = m_offset + size;
		else
		{
			if(m_ownsBuffer)
				ReserveBuffer(newSize);
			else
				FLT_ASSERT(false);
		}
	}

	stream.Read(m_buffer, size);
	//memcpy(m_buffer + m_offset, buffer, size);

	m_offset += size;
}

//////////////////////////////////////////////////////////////////////////

char* BufferStream::ReadString()
{
	u16 len;
	Read(&len, sizeof(u16));
	FLT_ASSERT(len < 1000);
	char* data = new char[len + 1];
	Read(data, len);
	data[len] = 0;
	return data;
}

//////////////////////////////////////////////////////////////////////////

int BufferStream::ReadString(char* s, int maxLength)
{
	u16 len;
	Read(&len, sizeof(u16));
	FLT_ASSERT(len < maxLength - 1);
	//char* data = new char[len + 1];
	Read(s, len);
	s[len] = 0;
	return len;
}

}