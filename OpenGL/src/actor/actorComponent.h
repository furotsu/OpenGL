/*
#pragma once
class TiXmlElement;

using ComponentId = unsigned long;
//---------------------------------------------------------------------------------------------------------------------
// ActorComponent class
// Chapter 6, page 167
//---------------------------------------------------------------------------------------------------------------------
class ActorComponent
{
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;
	// every implementation should have 
	// const static	ComponentId COMPONENT_ID	
public:
	virtual ~ActorComponent(void) { m_pOwner.reset(); }

	// These functions are meant to be overridden by the implementation classes of the components.
	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit(void) { }
	virtual void VUpdate(int deltaMs) { }
	//virtual void VOnChanged(void) { }

	//virtual TiXmlElement* VGenerateXml(void) = 0;

	virtual ComponentId VGetId(void) const = 0;

private:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }

};
*/