#include "ActorComponent.h"
#include "Objects/Actor.h"


Actor* ActorComponent::GetOwner()
{
	return pOwner;
}

void ActorComponent::SetOwner(Actor* owner)
{
	pOwner = owner;
}

void ActorComponent::Destruct()
{
	pOwner->UnregisterComponent(this);
	EndPlay();
	delete this;
}

void ActorComponent::Activate()
{
	OnActivated.BroadCast();
	bIsEnabled = true;
}

void ActorComponent::Deactivate()
{
	OnDeactivated.BroadCast();
	bIsEnabled = false;
}
