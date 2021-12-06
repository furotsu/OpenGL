#pragma once
/*
#include <map>
#include <vector>

//#include "ZipFile.h"			// needed for ZipContentsMap
#include "Interfaces.h"
#include "debugger.h"

class ResHandle;
class ResCache;

bool wildCardEqual(const std::string &pat,const std::string &str)
{
	int star_index = -1;
	int matched_upto = -1;
	int m = pat.size();
	int n = str.size();
	int i = 0, j = 0;
	while (i < n)
	{
		if (j < m and (str[i] == pat[j] or pat[j] == '?'))
		{
			i++;
			j++;
		}
		else if (j < m and pat[j] == '*')
		{
			star_index = j++;
			matched_upto = i;
		}
		else if (star_index != -1)
		{
			j = star_index + 1;
			matched_upto++;
			i = matched_upto;
		}
		else return 0;
	}
	while (j < m and pat[j] == '*') j++;
	return (j == m) ? 1 : 0;
}

class IResourceExtraData
{
public:
	virtual std::string VToString() = 0;
};

class Resource
{
public:
	std::string m_name;
	Resource(const std::string& name);
};

class ResHandle
{
	friend class ResCache;

protected:
	Resource m_resource;
	char* m_buffer;
	unsigned int m_size;
	std::shared_ptr<IResourceExtraData> m_extra;
	ResCache* m_pResCache;

public:
	ResHandle(Resource& resource, char* buffer, unsigned int size, ResCache* pResCache);

	virtual ~ResHandle();

	const std::string GetName() { return m_resource.m_name; }
	unsigned int Size() const { return m_size; }
	char* Buffer() const { return m_buffer; }
	char* WritableBuffer() { return m_buffer; }

	std::shared_ptr<IResourceExtraData> GetExtra() { return m_extra; }
	void SetExtra(std::shared_ptr<IResourceExtraData> extra) { m_extra = extra; }
};

class DefaultResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return true; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle) { return true; }
	virtual std::string VGetPattern() { return "*"; }

};

using ResHandleList = std::list<std::shared_ptr<ResHandle>>;					// lru list
using ResHandleMap = std::map<std::string, std::shared_ptr<ResHandle>>;		// maps indentifiers to resource data
using ResourceLoaders = std::list<std::shared_ptr<IResourceLoader>>;

class ResCache
{
	friend class ResHandle;

	ResHandleList m_lru;							// lru list
	ResHandleMap m_resources;
	ResourceLoaders m_resourceLoaders;

	IResourceFile* m_file;

	unsigned int			m_cacheSize;			// total memory size
	unsigned int			m_allocated;			// total memory allocated

protected:

	bool MakeRoom(unsigned int size);
	char* Allocate(unsigned int size);
	void Free(std::shared_ptr<ResHandle> gonner);

	std::shared_ptr<ResHandle> Load(Resource* r);
	std::shared_ptr<ResHandle> Find(Resource* r);
	void Update(std::shared_ptr<ResHandle> handle);

	void FreeOneResource();
	void MemoryHasBeenFreed(unsigned int size);

public:
	ResCache(const unsigned int sizeInMb, IResourceFile* file);
	virtual ~ResCache();

	bool Init();

	void RegisterLoader(std::shared_ptr<IResourceLoader> loader);

	std::shared_ptr<ResHandle> GetHandle(Resource* r);

	int Preload(const std::string pattern, void (*progressCallback)(int, bool&));
	std::vector<std::string> Match(const std::string pattern);

	void Flush(void);

	bool IsUsingDevelopmentDirectories(void) const { ASSERT(m_file); return m_file->VIsUsingDevelopmentDirectories(); }

};










//
//  class ResourceZipFile	- not discussed in the book
//
//    This class implements the IResourceFile interface with a ZipFile.
/*
class ResourceZipFile : public IResourceFile
{
	ZipFile* m_pZipFile;
	std::wstring m_resFileName;

public:
	ResourceZipFile(const std::wstring resFileName) { m_pZipFile = NULL; m_resFileName = resFileName; }
	virtual ~ResourceZipFile();

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource& r);
	virtual int VGetRawResource(const Resource& r, char* buffer);
	virtual int VGetNumResources() const;
	virtual std::string VGetResourceName(int num) const;
	virtual bool VIsUsingDevelopmentDirectories(void) const { return false; }
};

//
// class DevelopmentResourceZipFile							- not discussed in the book
//
//    This class fakes a ZIP file from a normal directory, and is used in the 
//    editor.
//
class DevelopmentResourceZipFile : public ResourceZipFile
{
public:
	enum Mode
	{
		Development,	// this mode checks the original asset directory for changes - helps during development
		Editor			// this mode only checks the original asset directory - the ZIP file is left unopened.
	};

	Mode m_mode;
	std::wstring m_AssetsDir;
	std::vector<WIN32_FIND_DATA> m_AssetFileInfo;
	ZipContentsMap m_DirectoryContentsMap;

	DevelopmentResourceZipFile(const std::wstring resFileName, const Mode mode);

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource& r);
	virtual int VGetRawResource(const Resource& r, char* buffer);
	virtual int VGetNumResources() const;
	virtual std::string VGetResourceName(int num) const;
	virtual bool VIsUsingDevelopmentDirectories(void) const { return true; }

	int Find(const std::string& path);

protected:
	void ReadAssetsDirectory(std::wstring fileSpec);
};
*/