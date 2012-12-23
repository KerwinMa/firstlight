#ifndef FLT_ARCHIVE_MANAGER_H
#define FLT_ARCHIVE_MANAGER_H

#include "utils/ISingleton.h"

namespace flt
{
class IArchive;
class FileStream;

class ArchiveManager: public utils::ISingleton<ArchiveManager>
{
public:
	ArchiveManager(){};
	~ArchiveManager();

	void openFile(FileStream& filestream, const stringc& fileName, const stringc& archiveName = "");

	FileStream* openFile(const stringc& fileName, const stringc& archiveName = "");

	IArchive* load( const stringc& archiveName, const int archiveType);

	void unload(IArchive* arch);

	void unload(const stringc& archiveName);

protected:
	typedef std::map<stringc, IArchive*> ArchiveMap;
	ArchiveMap mArchives;
};

}//end namespace flt

#endif //FLT_ARCHIVE_MANAGER_H