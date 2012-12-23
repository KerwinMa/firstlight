#include "firstlight.h"

#include <sys/types.h>
#include <sys/stat.h>

#if defined (FLT_DEVICE_WIN32)
#  define WIN32_LEAN_AND_MEAN
#  define NOMINMAX // required to stop windows.h messing up std::min
#  include <windows.h>
#  include <direct.h>
#  include <io.h>
#elif defined (FLT_DEVICE_IOS)
#  include <sys/param.h>
#  include <dirent.h>
#  include <unistd.h>
#  include <fnmatch.h>
#endif


namespace flt
{

#if defined (FLT_DEVICE_IOS)	
	
#define _A_NORMAL 0x00  /* Normalfile-Noread */
#define _A_RDONLY 0x01  /* Read only file */
#define _A_HIDDEN 0x02  /* Hidden file */
#define _A_SYSTEM 0x04  /* System file */
#define _A_SUBDIR 0x10  /* Subdirectory */
	
	struct _finddata_t
	{
		char *name;
		int attrib;
		unsigned long size;
	};	
	
	
	struct _find_search_t
	{
		char *pattern;
		char *curfn;
		char *directory;
		int dirlen;
		DIR *dirfd;
	};
	
	
	int _findnext(long id, struct _finddata_t *data)
	{
		_find_search_t *fs = (_find_search_t *)id;
		
		/* Loop until we run out of entries or find the next one */
		dirent *entry;
		for (;;)
		{
			if (!(entry = readdir (fs->dirfd)))
				return -1;
			
			/* See if the filename matches our pattern */
			if (fnmatch (fs->pattern, entry->d_name, 0) == 0)
				break;
		}
		
		if (fs->curfn)
			free (fs->curfn);
		data->name = fs->curfn = strdup (entry->d_name);
		
		size_t namelen = strlen (entry->d_name);
		char *xfn = new char [fs->dirlen + 1 + namelen + 1];
		sprintf (xfn, "%s/%s", fs->directory, entry->d_name);
		
		/* stat the file to get if it's a subdir and to find its length */
		struct stat stat_buf;
		if (stat (xfn, &stat_buf))
		{
			// Hmm strange, imitate a zero-length file then
			data->attrib = _A_NORMAL;
			data->size = 0;
		}
		else
		{
			if (S_ISDIR(stat_buf.st_mode))
				data->attrib = _A_SUBDIR;
			else
            /* Default type to a normal file */
				data->attrib = _A_NORMAL;
			
			data->size = stat_buf.st_size;
		}
		
		delete [] xfn;
		
		/* Files starting with a dot are hidden files in Unix */
		if (data->name [0] == '.')
			data->attrib |= _A_HIDDEN;
		
		return 0;
	}
	
	int _findclose(long id)
	{
		int ret;
		_find_search_t *fs = (_find_search_t *)id;
		
		ret = fs->dirfd ? closedir (fs->dirfd) : 0;
		free (fs->pattern);
		free (fs->directory);
		if (fs->curfn)
			free (fs->curfn);
		delete fs;
		
		return ret;
	}
	
	long _findfirst(const char *pattern, struct _finddata_t *data)
	{
		_find_search_t *fs = new _find_search_t;
		fs->curfn = NULL;
		fs->pattern = NULL;
		
		// Separate the mask from directory name
		const char *mask = strrchr (pattern, '/');
		if (mask)
		{
			fs->dirlen = mask - pattern;
			mask++;
			fs->directory = (char *)malloc (fs->dirlen + 1);
			memcpy (fs->directory, pattern, fs->dirlen);
			fs->directory [fs->dirlen] = 0;
		}
		else
		{
			mask = pattern;
			fs->directory = strdup (".");
			fs->dirlen = 1;
		}
		
		fs->dirfd = opendir (fs->directory);
		if (!fs->dirfd)
		{
			_findclose ((long)fs);
			return -1;
		}
		
		/* Hack for "*.*" -> "*' from DOS/Windows */
		if (strcmp (mask, "*.*") == 0)
			mask += 2;
		fs->pattern = strdup (mask);
		
		/* Get the first entry */
		if (_findnext ((long)fs, data) < 0)
		{
			_findclose ((long)fs);
			return -1;
		}
		
		return (long)fs;
	}
	
#endif	
	
	bool FileSystemArchive::ms_IgnoreHidden = true;

	//-----------------------------------------------------------------------
	FileSystemArchive::FileSystemArchive(const stringc& name)
		: IArchive(name, archive_type::FILE_SYSTEM)
	{
	}
	//-----------------------------------------------------------------------
	bool FileSystemArchive::isCaseSensitive(void) const
	{
#if defined (FLT_DEVICE_WIN32)
		return false;
#else
		return true;
#endif

	}
	//-----------------------------------------------------------------------
	static bool is_reserved_dir (const char *fn)
	{
		return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
	}
	//-----------------------------------------------------------------------
	static bool is_absolute_path(const char* path)
	{
#if defined (FLT_DEVICE_WIN32)
		if (isalpha(u8(path[0])) && path[1] == ':')
			return true;
#endif
		return path[0] == '/' || path[0] == '\\';
	}
	//-----------------------------------------------------------------------
	static stringc concatenate_path(const stringc& base, const stringc& name)
	{
		if (base.empty() || is_absolute_path(name.c_str()))
			return name;
		else
			return base + '/' + name;
	}
	//-----------------------------------------------------------------------
	void FileSystemArchive::findFiles(const stringc& pattern, bool recursive, 
		bool dirs, StringVector* simpleList, FileInfoList* detailList)
	{
		long lHandle, res;
		struct _finddata_t tagData;

		// pattern can contain a directory name, separate it from mask
		size_t pos1 = pattern.rfind ('/');
		size_t pos2 = pattern.rfind ('\\');
		if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
			pos1 = pos2;
		stringc directory;
		if (pos1 != pattern.npos)
			directory = pattern.substr (0, pos1 + 1);

		stringc full_pattern = concatenate_path(getRootPath() + m_name, pattern);

		lHandle = (long)_findfirst(full_pattern.c_str(), &tagData);
		res = 0;
		while (lHandle != -1 && res != -1)
		{
			if ((dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
				( !ms_IgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
				(!dirs || !is_reserved_dir (tagData.name)))
			{
				if (simpleList)
				{
					simpleList->push_back(directory + tagData.name);
				}
				else if (detailList)
				{
					FileInfo fi;
					fi.archive = this;
					fi.filename = directory + tagData.name;
					fi.basename = tagData.name;
					fi.path = directory;
					fi.compressedSize = tagData.size;
					fi.uncompressedSize = tagData.size;
					detailList->push_back(fi);
				}
			}
			res = _findnext( lHandle, &tagData );
		}
		// Close if we found any files
		if(lHandle != -1)
			_findclose(lHandle);

		// Now find directories
		if (recursive)
		{
			stringc base_dir = m_name;
			if (!directory.empty ())
			{
				base_dir = concatenate_path(getRootPath() + m_name, directory);
				// Remove the last '/'
				base_dir.erase (base_dir.length () - 1);
			}
			base_dir.append ("/*");

			// Remove directory name from pattern
			stringc mask ("/");
			if (pos1 != pattern.npos)
				mask.append (pattern.substr (pos1 + 1));
			else
				mask.append (pattern);

			lHandle = (long)_findfirst(base_dir.c_str (), &tagData);
			res = 0;
			while (lHandle != -1 && res != -1)
			{
				if ((tagData.attrib & _A_SUBDIR) &&
					( !ms_IgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
					!is_reserved_dir (tagData.name))
				{
					// recurse
					base_dir = directory;
					base_dir.append (tagData.name).append (mask);
					findFiles(base_dir, recursive, dirs, simpleList, detailList);
				}
				res = _findnext( lHandle, &tagData );
			}
			// Close if we found any files
			if(lHandle != -1)
				_findclose(lHandle);
		}
	}
	//-----------------------------------------------------------------------
	FileSystemArchive::~FileSystemArchive()
	{
		unload();
	}
	//-----------------------------------------------------------------------
	void FileSystemArchive::load()
	{
		// do nothing here, what has to be said will be said later
	}
	//-----------------------------------------------------------------------
	void FileSystemArchive::unload()
	{
		// nothing to see here, move along
	}
	//-----------------------------------------------------------------------
	void FileSystemArchive::open(const stringc& filename, FileStream& fileStream)
	{
		stringc full_path = concatenate_path(getRootPath() + m_name, filename);

		// Use filesystem to determine size 
		// (quicker than streaming to the end and back)
		struct stat tagStat;
		int ret = stat(full_path.c_str(), &tagStat);
		assert(ret == 0 && "Problem getting file size" );

		fileStream.Open(full_path.c_str());

		// Should check ensure open succeeded, in case fail for some reason.
		if (!fileStream.IsValid())
		{
			LOG("engine","Cannot open file: %s" , filename.c_str());
		}
	}
	//-----------------------------------------------------------------------
	FileStream* FileSystemArchive::open(const stringc& filename)
	{
		stringc full_path = concatenate_path(getRootPath() + m_name, filename);

		// Use filesystem to determine size 
		// (quicker than streaming to the end and back)
		struct stat tagStat;
		int ret = stat(full_path.c_str(), &tagStat);
		assert(ret == 0 && "Problem getting file size" );

		FileStream* fileStream = new FileStream(full_path.c_str());

		// Should check ensure open succeeded, in case fail for some reason.
		if (!fileStream->IsValid())
		{
			LOG("engine","Cannot open file: %s" , filename.c_str());
		}

		return fileStream;
	}
	//-----------------------------------------------------------------------
	StringVector* FileSystemArchive::list(bool recursive, bool dirs)
	{
		StringVector* ret = new StringVector();

		findFiles("*", recursive, dirs, ret, 0);

		return ret;
	}
	//-----------------------------------------------------------------------
	FileInfoList* FileSystemArchive::listFileInfo(bool recursive, bool dirs)
	{
		FileInfoList* ret = new FileInfoList();

		findFiles("*", recursive, dirs, 0, ret);

		return ret;
	}
	//-----------------------------------------------------------------------
	StringVector* FileSystemArchive::find(const stringc& pattern,
		bool recursive, bool dirs)
	{
		StringVector* ret = new StringVector();

		findFiles(pattern, recursive, dirs, ret, 0);

		return ret;

	}
	//-----------------------------------------------------------------------
	FileInfoList* FileSystemArchive::findFileInfo(const stringc& pattern, 
		bool recursive, bool dirs)
	{
		FileInfoList* ret = new FileInfoList();

		findFiles(pattern, recursive, dirs, 0, ret);

		return ret;
	}
	//-----------------------------------------------------------------------
	bool FileSystemArchive::exists(const stringc& filename)
	{
		stringc full_path = concatenate_path(getRootPath() + m_name, filename);

		struct stat tagStat;
		bool ret = (stat(full_path.c_str(), &tagStat) == 0);

		// stat will return true if the filename is absolute, but we need to check
		// the file is actually in this archive
		if (ret && is_absolute_path(filename.c_str()))
		{
			// only valid if full path starts with our base
#if defined (FLT_DEVICE_WIN32)
			// case insensitive on windows
			stringc lowerCaseName = m_name;
			StringUtil::toLowerCase(lowerCaseName);
			ret = StringUtil::startsWith(full_path, lowerCaseName, true);
#else
			// case sensitive
			ret = StringUtil::startsWith(full_path, m_name, false);
#endif
		}

		return ret;
	}
	//---------------------------------------------------------------------
	time_t FileSystemArchive::getModifiedTime(const stringc& filename)
	{
		stringc full_path = concatenate_path(getRootPath() + m_name, filename);

		struct stat tagStat;
		bool ret = (stat(full_path.c_str(), &tagStat) == 0);

		if (ret)
		{
			return tagStat.st_mtime;
		}
		else
		{
			return 0;
		}

	}

#if defined (FLT_DEVICE_WIN32)
	stringc FileSystemArchive::getRootPath()
	{ 
		return ""; 
	}
#endif

} //namespace flt

