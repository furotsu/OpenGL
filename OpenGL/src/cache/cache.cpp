/*#include <list>
#include <map>
#include <cctype>
#include <algorithm>

#include "debugger.h"
#include "cache.h"

#if !defined(SAFE_DELETE)
#define SAFE_DELETE(x) if(x) delete x; x=NULL;
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE(x) if(x) x->Release(); x=NULL;
#endif

Resource::Resource(const std::string& name)
{
	m_name = name;
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int)) std::tolower);
}

ResHandle::ResHandle(Resource& resource, char* buffer, unsigned int size, ResCache* pResCache)
	: m_resource(resource)
{
	m_buffer = buffer;
	m_size = size;
	m_extra = NULL;
	m_pResCache = pResCache;
}

ResHandle::~ResHandle()
{
	SAFE_DELETE_ARRAY(m_buffer);
	m_pResCache->MemoryHasBeenFreed(m_size);
}

ResCache::ResCache(const unsigned int sizeInMb, IResourceFile* resFile)
{
	m_cacheSize = sizeInMb * 1024 * 1024;				// total memory size
	m_allocated = 0;									// total memory allocated
	m_file = resFile;
}

ResCache::~ResCache()
{
	while (!m_lru.empty())
	{
		FreeOneResource();
	}
	SAFE_DELETE(m_file);
}

bool ResCache::Init()
{
	bool retValue = false;
	if (m_file->VOpen())
	{
		RegisterLoader(std::shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
		retValue = true;
	}
	return retValue;
}

void ResCache::RegisterLoader(std::shared_ptr<IResourceLoader> loader)
{
	m_resourceLoaders.push_front(loader);
}

std::shared_ptr<ResHandle> ResCache::GetHandle(Resource* r)
{
	std::shared_ptr<ResHandle> handle(Find(r));
	if (handle == NULL)
	{
		handle = Load(r);
		ASSERT(handle);
	}
	else
	{
		Update(handle);
	}
	return handle;
}

std::shared_ptr<ResHandle> ResCache::Load(Resource* r)
{
	// Create a new resource and add it to the lru list and map

	std::shared_ptr<IResourceLoader> loader;
	std::shared_ptr<ResHandle> handle;

	for (ResourceLoaders::iterator it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it)
	{
		std::shared_ptr<IResourceLoader> tmpLoader = *it;

		if (wildCardEqual(tmpLoader->VGetPattern(), r->m_name))
		{
			loader = tmpLoader;
			break;
		}
	}

	if (!loader)
	{
		ASSERT(loader && "Default resource loader not found!");
		return handle;		// Resource not loaded!
	}

	int rawSize = m_file->VGetRawResourceSize(*r);
	if (rawSize < 0)
	{
		ASSERT(rawSize > 0 && "Resource size returned -1 - Resource not found");
		return std::shared_ptr<ResHandle>();
	}

	int allocSize = rawSize + ((loader->VAddNullZero()) ? (1) : (0));
	char* rawBuffer = loader->VUseRawFile() ? Allocate(allocSize) : new char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer == NULL || m_file->VGetRawResource(*r, rawBuffer) == 0)
	{
		// resource cache out of memory
		return std::shared_ptr<ResHandle>();
	}

	char* buffer = NULL;
	unsigned int size = 0;

	if (loader->VUseRawFile())
	{
		buffer = rawBuffer;
		handle = std::shared_ptr<ResHandle>(new ResHandle(*r, buffer, rawSize, this));
	}
	else
	{
		size = loader->VGetLoadedResourceSize(rawBuffer, rawSize);
		buffer = Allocate(size);
		if (rawBuffer == NULL || buffer == NULL)
		{
			// resource cache out of memory
			return std::shared_ptr<ResHandle>();
		}
		handle = std::shared_ptr<ResHandle>(new ResHandle(*r, buffer, size, this));
		bool success = loader->VLoadResource(rawBuffer, rawSize, handle);

		// [mrmike] - This was added after the chapter went to copy edit. It is used for those
		//            resoruces that are converted to a useable format upon load, such as a compressed
		//            file. If the raw buffer from the resource file isn't needed, it shouldn't take up
		//            any additional memory, so we release it.
		//
		if (loader->VDiscardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if (!success)
		{
			// resource cache out of memory
			return std::shared_ptr<ResHandle>();
		}
	}

	if (handle)
	{
		m_lru.push_front(handle);
		m_resources[r->m_name] = handle;
	}

	ASSERT(loader && "Default resource loader not found!");
	return handle;		// ResCache is out of memory!
}

std::shared_ptr<ResHandle> ResCache::Find(Resource* r)
{
	ResHandleMap::iterator i = m_resources.find(r->m_name);
	if (i == m_resources.end())
		return std::shared_ptr<ResHandle>();

	return i->second;
}

void ResCache::Update(std::shared_ptr<ResHandle> handle)
{
	m_lru.remove(handle);
	m_lru.push_front(handle);
}

char* ResCache::Allocate(unsigned int size)
{
	if (!MakeRoom(size))
		return NULL;

	char* mem = new char[size];
	if (mem)
	{
		m_allocated += size;
	}

	return mem;
}

void ResCache::FreeOneResource()
{
	ResHandleList::iterator gonner = m_lru.end();
	gonner--;

	std::shared_ptr<ResHandle> handle = *gonner;

	m_lru.pop_back();
	m_resources.erase(handle->m_resource.m_name);
	// Note - you can't change the resource cache size yet - the resource bits could still actually be
	// used by some sybsystem holding onto the ResHandle. Only when it goes out of scope can the memory
	// be actually free again.
}

void ResCache::Flush()
{
	while (!m_lru.empty())
	{
		std::shared_ptr<ResHandle> handle = *(m_lru.begin());
		Free(handle);
		m_lru.pop_front();
	}
}

bool ResCache::MakeRoom(unsigned int size)
{
	if (size > m_cacheSize)
	{
		return false;
	}

	// return null if there's no possible way to allocate the memory
	while (size > (m_cacheSize - m_allocated))
	{
		// The cache is empty, and there's still not enough room.
		if (m_lru.empty())
			return false;

		FreeOneResource();
	}

	return true;
}

void ResCache::Free(std::shared_ptr<ResHandle> gonner)
{
	m_lru.remove(gonner);
	m_resources.erase(gonner->m_resource.m_name);
	// Note - the resource might still be in use by something,
	// so the cache can't actually count the memory freed until the
	// ResHandle pointing to it is destroyed.

	//m_allocated -= gonner->m_resource.m_size;
	//delete gonner;
}

void ResCache::MemoryHasBeenFreed(unsigned int size)
{
	m_allocated -= size;
}

std::vector<std::string> ResCache::Match(const std::string pattern)
{
	std::vector<std::string> matchingNames;
	if (m_file == NULL)
		return matchingNames;

	int numFiles = m_file->VGetNumResources();
	for (int i = 0; i < numFiles; ++i)
	{
		std::string name = m_file->VGetResourceName(i);
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if (wildCardEqual(pattern, name))
		{
			matchingNames.push_back(name);
		}
	}
	return matchingNames;
}

int ResCache::Preload(const std::string pattern, void (*progressCallback)(int, bool&))
{
	if (m_file == NULL)
		return 0;

	int numFiles = m_file->VGetNumResources();
	int loaded = 0;
	bool cancel = false;
	for (int i = 0; i < numFiles; ++i)
	{
		Resource resource(m_file->VGetResourceName(i));

		if (wildCardEqual(pattern, resource.m_name))
		{
			std::shared_ptr<ResHandle> handle = g_pApp->m_ResCache->GetHandle(&resource);
			++loaded;
		}

		if (progressCallback != NULL)
		{
			progressCallback(i * 100 / numFiles, cancel);
		}
	}
	return loaded;
}


/*
ResourceZipFile::~ResourceZipFile()
{
	SAFE_DELETE(m_pZipFile);
}


bool ResourceZipFile::VOpen()
{
	m_pZipFile = GCC_NEW ZipFile;
	if (m_pZipFile)
	{
		return m_pZipFile->Init(m_resFileName.c_str());
	}
	return false;
}



int ResourceZipFile::VGetRawResourceSize(const Resource& r)
{
	int resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum == -1)
		return -1;

	return m_pZipFile->GetFileLen(resourceNum);
}



int ResourceZipFile::VGetRawResource(const Resource& r, char* buffer)
{
	int size = 0;
	optional<int> resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum.valid())
	{
		size = m_pZipFile->GetFileLen(*resourceNum);
		m_pZipFile->ReadFile(*resourceNum, buffer);
	}
	return size;
}



int ResourceZipFile::VGetNumResources() const
{
	return (m_pZipFile == NULL) ? 0 : m_pZipFile->GetNumFiles();
}




std::string ResourceZipFile::VGetResourceName(int num) const
{
	std::string resName = "";
	if (m_pZipFile != NULL && num >= 0 && num < m_pZipFile->GetNumFiles())
	{
		resName = m_pZipFile->GetFilename(num);
	}
	return resName;
}

DevelopmentResourceZipFile::DevelopmentResourceZipFile(const std::wstring resFileName, const Mode mode)
	: ResourceZipFile(resFileName)
{
	m_mode = mode;

	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	m_AssetsDir = dir;
	int lastSlash = m_AssetsDir.find_last_of(L"\\");
	m_AssetsDir = m_AssetsDir.substr(0, lastSlash);
	m_AssetsDir += L"\\Assets\\";
}

int DevelopmentResourceZipFile::Find(const std::string& name)
{
	std::string lowerCase = name;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);
	ZipContentsMap::const_iterator i = m_DirectoryContentsMap.find(lowerCase);
	if (i == m_DirectoryContentsMap.end())
		return -1;

	return i->second;
}



bool DevelopmentResourceZipFile::VOpen()
{
	if (m_mode != Editor)
	{
		ResourceZipFile::VOpen();
	}

	// open the asset directory and read in the non-hidden contents
	if (m_mode == Editor)
	{
		ReadAssetsDirectory(L"*");
	}
	else
	{
		// FUTURE WORK - iterate through the ZipFile contents and go grab WIN32_FIND_DATA 
		//   elements for each one. Then it would be possible to compare the dates/times of the
		//   asset in the Zip file with the source asset.
		GCC_ASSERT(0 && "Not implemented yet");
	}

	return true;
}


int DevelopmentResourceZipFile::VGetRawResourceSize(const Resource& r)
{
	int size = 0;

	if (m_mode == Editor)
	{
		int num = Find(r.m_name.c_str());
		if (num == -1)
			return -1;

		return m_AssetFileInfo[num].nFileSizeLow;
	}

	return ResourceZipFile::VGetRawResourceSize(r);
}

int DevelopmentResourceZipFile::VGetRawResource(const Resource& r, char* buffer)
{
	if (m_mode == Editor)
	{
		int num = Find(r.m_name.c_str());
		if (num == -1)
			return -1;

		std::string fullFileSpec = ws2s(m_AssetsDir) + r.m_name.c_str();
		FILE* f = fopen(fullFileSpec.c_str(), "rb");
		size_t bytes = fread(buffer, 1, m_AssetFileInfo[num].nFileSizeLow, f);
		fclose(f);
		return bytes;
	}

	return ResourceZipFile::VGetRawResource(r, buffer);
}

int DevelopmentResourceZipFile::VGetNumResources() const
{
	return (m_mode == Editor) ? m_AssetFileInfo.size() : ResourceZipFile::VGetNumResources();
}

std::string DevelopmentResourceZipFile::VGetResourceName(int num) const
{
	if (m_mode == Editor)
	{
		std::wstring wideName = m_AssetFileInfo[num].cFileName;
		return ws2s(wideName);
	}

	return ResourceZipFile::VGetResourceName(num);
}


void DevelopmentResourceZipFile::ReadAssetsDirectory(std::wstring fileSpec)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	// get first file
	std::wstring pathSpec = m_AssetsDir + fileSpec;
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		// loop on all remeaining entries in dir
		while (FindNextFile(fileHandle, &findData))
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			std::wstring fileName = findData.cFileName;
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName + L"\\*";
					ReadAssetsDirectory(fileName);
				}
			}
			else
			{
				fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName;
				std::wstring lower = fileName;
				std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int)) std::tolower);
				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				m_DirectoryContentsMap[ws2s(lower)] = m_AssetFileInfo.size();
				m_AssetFileInfo.push_back(findData);
			}
		}
	}

	FindClose(fileHandle);
}

*/
