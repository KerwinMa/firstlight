#ifndef FLT_DATA_STREAM_H
#define FLT_DATA_STREAM_H

#include "Stream.h"

namespace flt
{

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

/// @brief A data stream lets an application read primitive data types from an underlying input stream in a machine-independent way.
///
/// Similar to Java DataInputStream and DataOutputStream combined classes.\n
/// Short and integer values will be swapped if the stream endian order is
/// different than System::IsBigEndian().

class DataStream : public Stream
{
public:
	/// @brief Construct a data stream (little endian by default)
	/// @param stream Parent stream to read/write data
	/// @param endian Big/Little endian
	DataStream(Stream& stream);
	virtual ~DataStream(){}

	virtual int Read(void* buffer, int size);
	virtual int Write(const void* buffer, int size);
	virtual void Seek(int offset, int origin);
	virtual int Tell() const;

	//----------
	// Input
	//----------
	/// @brief Skip a number of bytes. Equivalent to Stream::Seek(count, io::cur).
	/// @param count Number of bytes to skip
	void Skip(int count);
	
	/// @brief Read a boolean. Read a byte and return true if not 0.
	/// @return Boolean value.
	bool ReadBoolean();
	
	/// @brief Read a single byte. Equivalent to Stream::Read1().
	/// @return signed char value.
	signed char ReadByte();
	
	/// @brief Read a single byte and treat it as unsigned.
	/// @return Integer value.
	unsigned char ReadUnsignedByte();
	
	/// @brief Read a short (two bytes).
	/// @return Short value.
	short ReadShort();

	/// @brief Read a short (two bytes) and treat it as unsigned.
	/// @return Unsigned short value.
	unsigned short ReadUnsignedShort();

	// Java: char=2 bytes, C++: char=1 byte (removed because of confusion)
	//char ReadChar();
	
	/// @brief Read a integer (four bytes).
	/// @return Integer value.
	int ReadInt();

	// Java: long=4 bytes, C++: long=4 bytes (removed because of confusion)
	// @brief Read a integer (four bytes). Same as ReadInt().
	// @return Integer value.
	//long ReadLong();

	/// @brief Read a string into a buffer.
	/// @param buffer Buffer to receive string, should be long enough
	/// @param bufferLength Buffer length to receive string
	/// @return buffer pointer.
	char* ReadUTF(char* buffer, int bufferLength);

	//----------
	// Output
	//----------
	/// @brief Write a boolean. Equivalent to writing a single byte: 1 for true, 0 otherwize.
	void WriteBoolean(bool v);

	/// @brief Write a single byte. The integer value is casted.
	/// @param v Integer value.
	void WriteByte(int v);

	/// @brief Write a short (2 bytes). The integer value is casted.
	/// @param v Integer value.
	void WriteShort(int v);

	//void WriteChar(int v);
	
	/// @brief Write a integer (4 bytes).
	/// @param v Integer value.
	void WriteInt(int v);
	
	//void WriteLong(long v);
	
	/// @brief Write a UTF string: (short value for length + 1 byte/character).
	/// @param string String to write.
	void WriteUTF(const char* string);

protected:
	Stream& m_stream;

	inline short SwapShort(short v);
	inline int SwapInt(int v);

private:
	DataStream(const DataStream&);
	DataStream& operator=(const DataStream&);
};

short DataStream::SwapShort(short v)
{
	return	((v & 0x00ff) << 8) |
			((v & 0xff00) >> 8);
}

int DataStream::SwapInt(int v)
{
	return	((v & 0xff000000) >> 24) |
			((v & 0x00ff0000) >> 8) |
			((v & 0x0000ff00) << 8) |
			((v & 0x000000ff) << 24);
}

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif //FLT_DATA_STREAM_H