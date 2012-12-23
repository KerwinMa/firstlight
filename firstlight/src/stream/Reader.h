#ifndef FLT_STREAM_READER_H
#define FLT_STREAM_READER_H

#include "Stream.h"

namespace flt
{

/// @brief Read text from a character-input stream, buffering characters so as to provide for the efficient reading of characters, arrays, and lines.
class Reader
{
public:
	Reader(Stream& stream);

	/// @brief Read a single character.
	char Read();

	/// @brief Read characters into an array.
	int Read(char* cbuf, int len);
	
	/// @brief Read a line of text.
	///
	/// A line is considered to be terminated by any one of a line feed ('\n'), a carriage return ('\r'),
	/// or a carriage return followed immediately by a linefeed.
	/// @param cbuf Destination buffer
	/// @param buflen Length of the destination buffer
	/// @return The number of characters read
	int ReadLine(char* cbuf, int buflen);
	
	/// @brief Skip characters.
	void Skip(int n);

	Stream& m_stream;

private:
	Reader(const Reader&);
	Reader& operator=(const Reader&);
};

}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif