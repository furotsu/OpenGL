/*#pragma once

#include <memory>
#include <map>
#include <string>

#include "actor.h"
#include "actorComponent.h"

using ActorComponentCreator = ActorComponent*(*)(void);
using ActorComponentCreatorMap = std::map<std::string, ActorComponentCreator>;


using StrongActorPtr = std::shared_ptr<Actor>;
using StrongActorComponentPtr = std::shared_ptr<ActorComponent>;

class ActorFactory
{
    ActorId m_lastActorId;

protected:
     
    ActorComponentCreatorMap m_actorComponentCreators;
public:
    ActorFactory(void);

    StrongActorPtr CreateActor(const char* actorResource);// , TiXmlElement* overrides, const Mat4x4* initialTransform, const ActorId serversActorId);
    //void ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides);

    protected:
    virtual StrongActorComponentPtr CreateComponent(TiXmlElement* pData);

private:
    ActorId GetNextActorId(void) { ++m_lastActorId; return m_lastActorId; }
};
*/