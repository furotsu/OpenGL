/*

#include "XmlLoader.h"

void XmlResourceExtraData::ParseXml(char* pRawBuffer)
{
    m_xmlDocument.Parse(pRawBuffer);
}


bool XmlResourceLoader::VLoadResource(char* rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle)
{
    if (rawSize <= 0)
        return false;

    std::shared_ptr<XmlResourceExtraData> pExtraData = std::shared_ptr<XmlResourceExtraData>(new XmlResourceExtraData());
    pExtraData->ParseXml(rawBuffer);

    handle->SetExtra(std::shared_ptr<XmlResourceExtraData>(pExtraData));

    return true;
}


std::shared_ptr<IResourceLoader> CreateXmlResourceLoader()
{
    return std::shared_ptr<IResourceLoader>(new XmlResourceLoader());
}


TiXmlElement* XmlResourceLoader::LoadAndReturnRootXmlElement(const char* resourceString)
{
    Resource resource(resourceString);
    std::shared_ptr<ResHandle> pResourceHandle = g_pApp->m_ResCache->GetHandle(&resource);  // this actually loads the XML file from the zip file
    std::shared_ptr<XmlResourceExtraData> pExtraData = std::static_pointer_cast<XmlResourceExtraData>(pResourceHandle->GetExtra());
    return pExtraData->GetRoot();
}
*/