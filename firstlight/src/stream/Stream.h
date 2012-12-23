#ifndef FLT_STREAM_H
#define FLT_STREAM_H

namespace flt
{

/// @brief Constants to specify stream operations.
struct io
{
	static const int in		= 0x01;		///< Allow input operations on a stream.
	static const int out	= 0x02;		///< Allow output operations on a stream.
	static const int ate	= 0x04;		///< (at end) Seek to the end of the stream when opening.
	static const int app	= 0x08;		///< (append) Seek to the end of the stream before each output operation
	static const int trunc	= 0x10;		///< (truncate) Truncate file to zero when opening.
	static const int create = 0x20;		///< Create a stream.
	
	static const int beg	= 0;	///< Offset from the beginning of the stream's buffer. 
	static const int cur	= 1;	///< Offset from the current position in the stream's buffer. 
	static const int end	= 2;	///< Offset from the end of the stream's buffer.
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Base class for stream operations.
///
/// Unlike Java, GLLib streams are bi-directional (in and out).
/// This class can't be instanciated.

class Stream
{
public:
	Stream();
	virtual ~Stream(){}
	
	/// @brief Close this stream.
	virtual void Close();

	/// @brief Read buffer from stream.
	/// @param buffer Buffer to receive data
	/// @param buffer Size of data to read, in bytes
	/// @return Number of bytes read
	virtual int Read (void* buffer, int size) = 0;

	/// @brief Write buffer to stream.
	/// @param buffer Buffer to be written
	/// @param buffer Size of buffer to be written, in bytes
	/// @return Number of bytes written
	virtual int Write(const void* buffer, int size) = 0;
	
	/// @brief Seek stream to specified position.
	/// @param offset Offset position relative to origin
	/// @param origin io::beg, io::cur or io::end
	virtual void Seek(int offset, int origin) = 0;

	/// @brief Tells stream current position.
	virtual int  Tell() const = 0;

	/// @brief Read 4 bytes into an integer.
	//int	Read4();

	/// @brief Read 2 bytes into a short.
	//short Read2();

	/// @brief Read a single char.
	char Read();

	/// @brief Write an integer (4 bytes).
	/// @param four Integer to write
	//void Write4(int four);

	/// @brief Write a short (2 bytes).
	/// @param two Short to write
	//void Write2(short two);

	/// @brief Write a single byte.
	/// @param one Char to write
	void Write(char one);

	/// @brief Write the content of a stream, by streaming up to specified number of bytes.
	/// @param stream Stream to write
	/// @param size Number of bytes to read from stream to be written
	virtual void Write(Stream& stream, int size);

	/// @brief Tells if this stream is open.
	inline bool IsOpen() const;

protected:
	bool m_isOpen;

private:
	Stream(const Stream&);
	Stream& operator=(const Stream&);
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

bool Stream::IsOpen() const
{
	return m_isOpen;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

}

#endif
