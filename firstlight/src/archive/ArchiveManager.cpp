#include "firstlight.h"

namespace flt
{	
	ArchiveManager::~ArchiveManager()
	{
		LOG("engine","ArchiveManager released, clear all archives.");

		// Unload & delete resources in turn
		for( ArchiveMap::iterator it = mArchives.begin(); it != mArchives.end(); ++it )
		{
			IArchive* arch = it->second;
			// Unload
			arch->unload();
			delete arch;
		}
		// Empty the list
		mArchives.clear();
	}

	IArchive* ArchiveManager::load( const stringc& archiveName, const int archiveType)
	{
		ArchiveMap::iterator i = mArchives.find(archiveName);
		IArchive* pArch = 0;

		if (i == mArchives.end())
		{
			LOG("engine","Load Archive %s .", archiveName.c_str());

			if (archiveType == archive_type::FILE_SYSTEM)
			{
				pArch = new FileSystemArchive(archiveName);
				pArch->load();
				mArchives[archiveName] = pArch;
			}
			else if (archiveType == archive_type::ZIP)
			{

			}
		}
		else
		{
			pArch = i->second;
		}
		return pArch;
	}
	//-----------------------------------------------------------------------
	void ArchiveManager::unload(IArchive* arch)
	{
		unload(arch->getName());
	}
	//-----------------------------------------------------------------------
	void ArchiveManager::unload(const stringc& archiveName)
	{
		ArchiveMap::iterator i = mArchives.find(archiveName);

		if (i != mArchives.end())
		{
			LOG("engine","Unload Archive %s .", archiveName.c_str());

			i->second->unload();
			delete i->second;
			mArchives.erase(i);
		}
	}

	void ArchiveManager::openFile(FileStream& filestream, const stringc& fileName, const stringc& archiveName)
	{
		LOG("engine","Open File %s, in Archive: %s .", fileName.c_str(), archiveName.c_str());

		ArchiveMap::iterator i = mArchives.find(archiveName);

		if (i != mArchives.end())
		{
			IArchive* pArchive = i->second;
			if(pArchive->exists(fileName))
			{
				pArchive->open(fileName, filestream);
			}
		}
		else
		{
			for( ArchiveMap::iterator it = mArchives.begin(); it != mArchives.end(); ++it )
			{
				IArchive* pArchive = it->second;
				if(pArchive->exists(fileName))
				{
					pArchive->open(fileName, filestream);
				}
			}
		}
	}

	FileStream* ArchiveManager::openFile(const stringc& fileName, const stringc& archiveName)
	{
		LOG("engine","Open File %s, in Archive: %s .", fileName.c_str(), archiveName.c_str());

		ArchiveMap::iterator i = mArchives.find(archiveName);

		if (i != mArchives.end())
		{
			IArchive* pArchive = i->second;
			if(pArchive->exists(fileName))
			{
				return pArchive->open(fileName);
			}
		}
		else
		{
			for( ArchiveMap::iterator it = mArchives.begin(); it != mArchives.end(); ++it )
			{
				IArchive* pArchive = it->second;
				if(pArchive->exists(fileName))
				{
					return pArchive->open(fileName);
				}
			}
		}

		LOG("engine","Failed to open File %s, in Archive: %s .", fileName.c_str(), archiveName.c_str());

		return NULL;
	}
}