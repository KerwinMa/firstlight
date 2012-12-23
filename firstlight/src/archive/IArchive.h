#ifndef FLT_IARCHIVE_H
#define FLT_IARCHIVE_H

#include "common.h"

namespace flt
{
	struct FileInfo
	{
		IArchive* archive;
		/// The file's fully qualified name
		stringc filename;
		/// Path name; separated by '/' and ending with '/'
		stringc path;
		/// Base filename
		stringc basename;
		/// Compressed size
		size_t compressedSize;
		/// Uncompressed size
		size_t uncompressedSize;
	};

	typedef array_t<FileInfo> FileInfoList;
	typedef array_t<stringc> StringVector;

	class IArchive
	{
	public:
		IArchive(const stringc& name, int type) 
			:m_name(name),m_type(type) {}
		virtual ~IArchive() {}

		const stringc& getName(void) const { return m_name; }

		virtual bool isCaseSensitive(void) const = 0;

		virtual void load() = 0;

		virtual void unload() = 0;

		virtual void open(const stringc& filename, FileStream& filestream) = 0;

		virtual FileStream* open(const stringc& filename) = 0;

		virtual StringVector* list(bool recursive = true, bool dirs = false) = 0;

		virtual FileInfoList* listFileInfo(bool recursive = true, bool dirs = false) = 0;

		virtual StringVector* find(const stringc& pattern, bool recursive = true, bool dirs = false) = 0;

		virtual bool exists(const stringc& filename) = 0; 

		virtual time_t getModifiedTime(const stringc& filename) = 0; 

		virtual FileInfoList* findFileInfo(const stringc& pattern, bool recursive = true, bool dirs = false) = 0;

		virtual stringc getRootPath() { return ""; }
		
		/// Return the type code of this Archive
		const int getType(void) const { return m_type; }

	protected:

		stringc m_name;
		int	m_type;
	};

} //end namespace flt

#endif	//FLT_IARCHIVE_H