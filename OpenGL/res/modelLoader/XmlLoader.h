/*
#pragma once
#include <memory>

#include "tinyxml.h"
#include "cache/cache.h"

class XmlResourceExtraData
{
    TiXmlDocument m_xmlDocument;

public:
    virtual std::string VToString() { return "XmlResourceExtraData"; }
    void ParseXml(char* pRawBuffer);
    TiXmlElement* GetRoot(void) { return m_xmlDocument.RootElement(); }
};


class XmlResourceLoader
{
public:
    virtual bool VUseRawFile() { return false; }
    virtual bool VDiscardRawBufferAfterLoad() { return true; }
    virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
    virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle);
    virtual std::string VGetPattern() { return "*.xml"; }

    // convenience function
    static TiXmlElement* LoadAndReturnRootXmlElement(const char* resourceString);
};
*/