#include "firstlight.h"

namespace flt
{

//////////////////////////////////////////////////////////////////////////

Stream::Stream() :
	m_isOpen(false)
{

}

//////////////////////////////////////////////////////////////////////////

void Stream::Close()
{
	m_isOpen = false;
}

//////////////////////////////////////////////////////////////////////////

char Stream::Read()
{
	char one;

	int read = Read(&one, sizeof(one));

	FLT_ASSERT(read == sizeof(one));

	return one;
}

//////////////////////////////////////////////////////////////////////////

void Stream::Write(char one)
{
	int written = Write(&one, sizeof(one));

	FLT_ASSERT(written == sizeof(one));
}

//////////////////////////////////////////////////////////////////////////

void Stream::Write(Stream& stream, int size)
{
	const int bufSize = 1024;
	char buffer[bufSize];
	
	while(size != 0)
	{
		int toTransfert = core::min_(bufSize, size);
		stream.Read(buffer, toTransfert);
		Write(buffer, toTransfert);
		size -= toTransfert;
	}
}

}