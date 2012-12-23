#ifndef FLT_FILE_STREAM_H
#define FLT_FILE_STREAM_H

#include "Stream.h"

namespace flt
{

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

/// @brief Class for file system streaming operations.

class FileStream : public Stream
{
public:
	/// @brief Create a FileStream without openening the file.
	/// Open function should be used.
	FileStream();

	/// @brief Create and open a file for streaming operations.
	/// @see Open
	/// @param filename Filename to open.
	/// @param openMode 
	FileStream(const char* filename, int openMode = io::in);
	
	virtual ~FileStream();
	
	/// @brief Open a file for streaming operations.
	/// @param filename Filename to open.
	/// @param openMode io::in, io::out, io::create, io::app (can be OR'ed)
	bool Open(const char* filename, int openMode = io::in);
	virtual void Close();
	
	virtual int Read (void* buffer, int size);
	virtual int Write(const void* buffer, int size);
	
	virtual void Seek(int offset, int origin);
	virtual int  Tell() const;

	/// @brief Create and open a file for streaming operations.
	/// @param filename Filename to create.
	/// @return true if operation was successful.
	bool Create(const char* filename);

	/// @brief Test if the file is open.
	bool IsValid();

	/// @brief Get the file size, in bytes.
	int Size();

	const stringc& GetFileName() const
	{
		return m_fileName;
	}

	FILE* GetRawFile();

	/// @brief Test if the specified filename exists.
	/// @param filename File name to test
	/// @return true if file exist
	static bool Exist(const char* filename);

	/// @brief Delete a file from the file system.
	/// @param filename File name to delete
	/// @return true if operation was successful
	static bool Delete(const char* filename);

private:
	FileStream(const FileStream&);
	FileStream& operator=(const FileStream&);

	struct Impl;
	Impl* m_this; ///< Platform-specific data

	int m_size;

	stringc m_fileName;
};

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif //FLT_FILE_STREAM_H