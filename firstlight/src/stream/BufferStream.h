#ifndef FLT_BUFFER_STREAM_H
#define FLT_BUFFER_STREAM_H

#include "common.h"
#include "Stream.h"
#include <String.h>

namespace flt
{

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Read/write memory stream.
class BufferStream : public Stream
{
public:
	static const int BUFFER_INITIAL_SIZE = 65536;

	/// @brief Create a buffer stream which needs to be open.
	BufferStream();

	/// @brief Create a buffer stream, possibly from a specified buffer.
	/// This buffer stream becomes the owner of the buffer and is responsible to delete it.
	/// @see Open(int openMode, int size = BUFFER_INITIAL_SIZE, void* buffer = 0)
	BufferStream(int openMode, int size = BUFFER_INITIAL_SIZE, void* buffer = 0);

	/// @brief Create a buffer stream possibly from a specified buffer.
	/// @see Open(int openMode, int size, void* buffer, bool takeOwnerShip)
	BufferStream(int openMode, int size, void* buffer, bool takeOwnerShip);

	virtual ~BufferStream();

	/// @brief Initialize this buffer stream, possibly from a specified buffer.
	/// This buffer stream becomes the owner of the buffer and is responsible to delete it.
	/// @param openMode can be io::in, io::out, io::in|io::out
	/// @param size Size of the buffer, in bytes (default size is BUFFER_INITIAL_SIZE)
	/// @param buffer The buffer pointer (default is 0)
	void Open(int openMode, int size = BUFFER_INITIAL_SIZE, void* buffer = 0);
	
	/// @brief Initialize this buffer stream from a specified buffer, possibly taking ownership.
	/// @param openMode can be io::in, io::out, io::in|io::out
	/// @param size Size of the buffer, in bytes
	/// @param buffer The buffer pointer
	/// @param takeOwnerShip If true, this BufferStream becomes the owner of the buffer and is responsible to delete it
	void Open(int openMode, int size, void* buffer, bool takeOwnerShip);

	/// @brief Close this buffer stream.
	/// If the buffer is owned, it is deleted.
	virtual void Close();

	virtual int Read (void* buffer, int size);
	virtual int Write(const void* buffer, int size);

	inline void operator<<(double b) { Write(&b, sizeof(double)); };
	inline void operator<<(float b) { Write(&b, sizeof(float)); };
	inline void operator<<(bool b) { Write(&b, sizeof(bool)); };
	inline void operator<<(int i) { Write(&i, sizeof(int)); };
	inline void operator<<(u32 i) { Write(&i, sizeof(u32)); };
	inline void operator<<(s16 i) { Write(&i, sizeof(s16)); };
	inline void operator<<(u16 i) { Write(&i, sizeof(u16)); };
	inline void operator<<(s8 i) { Write(&i, sizeof(s8)); };
	inline void operator<<(u8 i) { Write(&i, sizeof(u8)); };
	inline void operator<<(const char* s) { int len = (int)strlen(s); Write(&len, 2); Write(s, len); };

	inline void operator>>(double &b) { Read(&b, sizeof(double)); };
	inline void operator>>(float &b) { Read(&b, sizeof(float)); };
	inline void operator>>(bool &b) { Read(&b, sizeof(bool)); };
	inline void operator>>(int &i) { Read(&i, sizeof(int)); };
	inline void operator>>(u32 &i) { Read(&i, sizeof(u32)); };
	inline void operator>>(s16 &i) { Read(&i, sizeof(s16)); };
	inline void operator>>(u16 &i) { Read(&i, sizeof(u16)); };
	inline void operator>>(s8 &i) { Read(&i, sizeof(s8)); };
	inline void operator>>(u8 &i) { Read(&i, sizeof(u8)); };
	inline void operator>>(char* s) { int len = 0; Read(&len, 2); Read(s, len); s[len] = 0; };

	virtual void Write(Stream& stream, int size);

	virtual void Seek(int offset, int origin = io::beg);
	int GetTotalSize(void);
	inline int Tell() const;

	/// @brief Get the pointer to buffer at the current position and seek a number of bytes from current position
	/// @param size The number of bytes to seek (size >= 0)
	/// @return Pointer to buffer at the current position
	void* GetBytes(int size);

	char* ReadString();
	int ReadString(char* s, int maxLength);

	//inline int Get4();
	//inline short Get2();
	//inline char Get1();

	/// @brief Get the buffer size
	inline int Size() const;

//protected:
	char m_defaultBuffer[BUFFER_INITIAL_SIZE];
	int m_openMode;
	char* m_buffer;
	int	m_size;
	int	m_capacity;
	int m_offset;
	bool m_ownsBuffer;

private:
	void ReserveBuffer(int newCapacity);

private:
	BufferStream(const BufferStream&);
	BufferStream& operator=(const BufferStream&);
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

//int BufferStream::Get4()
//{
//	FLT_ASSERT((Tell() & 3) == 0);
//
//	return *((int*)GetBytes(sizeof(int)));
//}
//
//short BufferStream::Get2()
//{
//	FLT_ASSERT((Tell() & 1) == 0);
//
//	return *((short*)GetBytes(sizeof(short)));
//}
//
//char BufferStream::Get1()
//{
//	return *((char*)GetBytes(sizeof(char)));
//}

int BufferStream::Tell() const
{
	return m_offset;
}

int BufferStream::Size() const
{
	return m_size;
}

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif //FLT_BUFFER_STREAM_H