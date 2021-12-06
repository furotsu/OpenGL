/*
#include "ActorFactory.h"
#include "debugger.h"

ActorFactory::ActorFactory(void)
{
    m_lastActorId = -1;
    /*
    m_componentFactory.Register<TransformComponent>(ActorComponent::GetIdFromName(TransformComponent::g_Name));
    m_componentFactory.Register<MeshRenderComponent>(ActorComponent::GetIdFromName(MeshRenderComponent::g_Name));
    m_componentFactory.Register<SphereRenderComponent>(ActorComponent::GetIdFromName(SphereRenderComponent::g_Name));
    m_componentFactory.Register<PhysicsComponent>(ActorComponent::GetIdFromName(PhysicsComponent::g_Name));
    m_componentFactory.Register<TeapotRenderComponent>(ActorComponent::GetIdFromName(TeapotRenderComponent::g_Name));
    m_componentFactory.Register<GridRenderComponent>(ActorComponent::GetIdFromName(GridRenderComponent::g_Name));
    m_componentFactory.Register<LightRenderComponent>(ActorComponent::GetIdFromName(LightRenderComponent::g_Name));
    m_componentFactory.Register<SkyRenderComponent>(ActorComponent::GetIdFromName(SkyRenderComponent::g_Name));
    m_componentFactory.Register<AudioComponent>(ActorComponent::GetIdFromName(AudioComponent::g_Name));

    // FUTURE WORK - certainly don't need to do this now, but the following stuff should be in a TeapotWarsActorFactory, eh?
    m_componentFactory.Register<AmmoPickup>(ActorComponent::GetIdFromName(AmmoPickup::g_Name));
    m_componentFactory.Register<HealthPickup>(ActorComponent::GetIdFromName(HealthPickup::g_Name));
    m_componentFactory.Register<BaseScriptComponent>(ActorComponent::GetIdFromName(BaseScriptComponent::g_Name));
    
}

StrongActorPtr ActorFactory::CreateActor(const char* actorResource)// , TiXmlElement* overrides, const Mat4x4* pInitialTransform, const ActorId serversActorId)
{
    TiXmlElement* pRoot = XmlResourceLoader::LoadAndReturnRootXmlElement(actorResource);
    if (!pRoot)
    {
        ASSERT(false && ("Failed to create actor from resource: " + std::string(actorResource)).c_str());
        return StrongActorPtr();
    }

    ActorId nextActorId = GetNextActorId();
    StrongActorPtr pActor(new Actor(nextActorId));
    if (!pActor->Init(pRoot))
    {
        ERROR("Failed to initialize actor: " + std::string(actorResource));
        return StrongActorPtr();
    }

    bool initialTransformSet = false;

    // Loop through each child element and load the component
    for (TiXmlElement* pNode = pRoot->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        StrongActorComponentPtr pComponent(CreateComponent(pNode));
        if (pComponent)
        {
            pActor->AddComponent(pComponent);
            pComponent->SetOwner(pActor);
        }
        else
        {
            // If error with component occurs we kill actor because it will be incomplete otherwise
            return StrongActorPtr();
        }
    }

    // Need to initialize ititial transform before other components, so they can 
    // be created with right paramenters
    //std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(pActor->GetComponent<TransformComponent>(TransformComponent::g_Name));
    //if (pInitialTransform && pTransformComponent)
    //{
    //    pTransformComponent->SetPosition(pInitialTransform->GetPosition());
    //}

    pActor->PostInit();

    return pActor;
}

StrongActorComponentPtr ActorFactory::CreateComponent(TiXmlElement* pData)
{
    std::string name(pData->Value());
    StrongActorComponentPtr pComponent; // (m_componentFactory.Create(ActorComponent::GetIdFromName(name)));

    auto creatorsIt = m_actorComponentCreators.find(name);
    if (creatorsIt != m_actorComponentCreators.end())
    {
        ActorComponentCreator creator = creatorsIt->second;
        pComponent.reset(creator);
    }
    // initialize the component if we found one
    if (pComponent)
    {
        if (!pComponent->VInit(pData))
        {
            ERROR("Component failed to initialize: " + std::string(name));
            return StrongActorComponentPtr();
        }
    }
    else
    {
        ERROR("Couldn't find ActorComponent named " + std::string(name));
        return StrongActorComponentPtr();
    }

    return pComponent;
}

/*
void ActorFactory::ModifyActor(StrongActorPtr pActor, TiXmlElement* overrides)
{
    // Loop through each child element and load the component
    for (TiXmlElement* pNode = overrides->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
    {
        ComponentId componentId = ActorComponent::GetIdFromName(pNode->Value());
        StrongActorComponentPtr pComponent = MakeStrongPtr(pActor->GetComponent<ActorComponent>(componentId));
        if (pComponent)
        {
            pComponent->VInit(pNode);

            // [mrmike] - added post press to ensure that components that need it have
            //            Events generated that can notify subsystems when changes happen.
            //            This was done to have SceneNode derived classes respond to RenderComponent
            //            changes.

            pComponent->VOnChanged();
        }
        else
        {
            pComponent = VCreateComponent(pNode);
            if (pComponent)
            {
                pActor->AddComponent(pComponent);
                pComponent->SetOwner(pActor);
            }
        }
    }
}
*/

