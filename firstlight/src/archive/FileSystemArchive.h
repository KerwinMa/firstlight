#ifndef FLT_FILE_SYSTEM_ARCHIVE_H
#define FLT_FILE_SYSTEM_ARCHIVE_H

#include "IArchive.h"

namespace flt
{
	class FileSystemArchive : public IArchive
	{
	protected:
		/** Utility method to retrieve all files in a directory matching pattern.
		@param pattern File pattern
		@param recursive Whether to cascade down directories
		@param dirs Set to true if you want the directories to be listed
		instead of files
		@param simpleList Populated if retrieving a simple list
		@param detailList Populated if retrieving a detailed list
		@param currentDir The current directory relative to the base of the 
		archive, for file naming
		*/
		void findFiles(const stringc& pattern, bool recursive, bool dirs,
			StringVector* simpleList, FileInfoList* detailList);

	public:
		FileSystemArchive(const stringc& name);
		~FileSystemArchive();

		/// @copydoc Archive::isCaseSensitive
		bool isCaseSensitive(void) const;

		/// @copydoc Archive::load
		void load();
		/// @copydoc Archive::unload
		void unload();

		void open(const stringc& filename, FileStream& filestream);
		/// @copydoc Archive::open
		FileStream* open(const stringc& filename);

		/// @copydoc Archive::list
		StringVector* list(bool recursive = true, bool dirs = false);

		/// @copydoc Archive::listFileInfo
		FileInfoList* listFileInfo(bool recursive = true, bool dirs = false);

		/// @copydoc Archive::find
		StringVector* find(const stringc& pattern, bool recursive = true,
			bool dirs = false);

		/// @copydoc Archive::findFileInfo
		FileInfoList* findFileInfo(const stringc& pattern, bool recursive = true,
			bool dirs = false);

		/// @copydoc Archive::exists
		bool exists(const stringc& filename);

		/// @copydoc Archive::getModifiedTime
		time_t getModifiedTime(const stringc& filename);
		
		stringc getRootPath();

		/// Set whether filesystem enumeration will include hidden files or not.
		/// This should be called prior to declaring and/or initializing filesystem
		/// resource locations. The default is true (ignore hidden files).
		static void setIgnoreHidden(bool ignore)
		{
			ms_IgnoreHidden = ignore;
		}

		/// Get whether hidden files are ignored during filesystem enumeration.
		static bool getIgnoreHidden()
		{
			return ms_IgnoreHidden;
		}

		static bool ms_IgnoreHidden;
	};
}

#endif //FLT_FILE_SYSTEM_ARCHIVE_H