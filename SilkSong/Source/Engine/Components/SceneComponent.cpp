#include "SceneComponent.h"
#include "Objects/Actor.h"


void SceneComponent::process_Destruct()
{
	if (!children.empty())
	{
		for (auto& child : children)
		{
			child->process_Destruct();
		}
	}
	ActorComponent::Destruct();
}

void SceneComponent::AttachTo(SceneComponent* par, FAttachmentTransformRules rule)
{
	if (par)
	{
		par->children.insert(this);
		parent = par;
		SetOwner(par->pOwner);
		transformRule = rule;
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



FVector2D SceneComponent::GetWorldPosition() const
{
	if (transformRule.LocationRule == EAttachmentRule::KeepWorld)
	{
		return GetLocalPosition();
	}

	if (parent)
	{
		return parent->GetWorldPosition() + FVector2D::RotateVector(parent->GetWorldRotation(), GetLocalPosition() * parent->GetWorldScale());
	}
	else
	{
		if (pOwner)return pOwner->GetWorldPosition();
		else return GetLocalPosition();
	}
}

float SceneComponent::GetWorldRotation() const
{
	if (transformRule.RotationRule == EAttachmentRule::KeepWorld)
	{
		return GetLocalRotation();
	}

	if (parent)
	{
		return parent->GetWorldRotation() + GetLocalRotation();
	}
	else
	{
		if (pOwner)return pOwner->GetWorldRotation();
		else return GetLocalRotation();
	}
}

FVector2D SceneComponent::GetWorldScale() const
{
	if (transformRule.ScaleRule == EAttachmentRule::KeepWorld)
	{
		return GetLocalScale();
	}

	if (parent && transformRule.ScaleRule == EAttachmentRule::KeepRelative)
	{
		return parent->GetWorldScale() * GetLocalScale();
	}
	else
	{
		if (pOwner)return pOwner->GetWorldScale();
		else return GetLocalScale();
	}
}