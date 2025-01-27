#include "SceneComponent.h"
#include "Objects/Actor.h"


void SceneComponent::process_Destruct()
{
	if (!children.empty())
		for (auto& child : children)child->process_Destruct();
	ActorComponent::Destruct();
}

void SceneComponent::AttachTo(SceneComponent* par)
{
	if (par)
	{
		par->children.insert(this);
		parent = par;
		SetOwner(par->pOwner);
	}
}


void SceneComponent::DetachFrom(SceneComponent* par)
{
	if (par)
	{
		par->children.erase(this);
		parent = nullptr;
	}
}

void SceneComponent::Destruct()
{
	if (parent)parent->children.erase(this);

	process_Destruct();
}



Vector2D SceneComponent::GetWorldPosition() const
{
	if (parent)
		return parent->GetWorldPosition() + GetLocalPosition();
	else
	{
		if (pOwner)return pOwner->GetWorldPosition();
		else return GetLocalPosition();
	}
}

float SceneComponent::GetWorldRotation() const
{
	if (parent)
		return parent->GetWorldRotation() + GetLocalRotation();
	else
	{
		if (pOwner)return pOwner->GetWorldRotation();
		else return GetLocalRotation();
	}

}

Vector2D SceneComponent::GetWorldScale() const
{
	if (parent)
		return parent->GetWorldScale() * GetLocalScale();
	else
	{
		if (pOwner)return pOwner->GetWorldScale();
		else return GetLocalScale();
	}
}