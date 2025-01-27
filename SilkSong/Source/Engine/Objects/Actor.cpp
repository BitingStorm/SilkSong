#include "Actor.h"
#include "Core/World.h"
#include "Components/SceneComponent.h"
#include "Components/Camera.h"
#include <easyx.h>
#include <stack>


Actor::Actor()
{
	SetRootComponent(new SceneComponent); root->SetOwner(this); RegisterComponent(root);
}

Actor::~Actor()
{
	for (auto& com : components)delete com;
}

void Actor::Update(float deltaTime)
{
	components_iter = components.begin();
	while (components_iter != components.end())
	{
		(*components_iter)->Update(deltaTime);
		if (components_iter != components.end())components_iter++;
	}
}

void Actor::BeginPlay()
{
	for (auto& it : components)
	{
		it->BeginPlay();
	}
}

void Actor::EndPlay()
{
	for (auto& it : components)
	{
		it->EndPlay();
	}
}

void Actor::SetRootComponent(SceneComponent* newRoot)
{
	const_cast<SceneComponent*&>(root) = newRoot;
}

void Actor::AttachTo(Actor* par)
{
	if (par)
	{
		par->children.insert(this);
		parent = par;
	}
}

void Actor::DetachFrom(Actor* par)
{
	if (par)
	{
		par->children.erase(this);
		parent = nullptr;
	}
}

void Actor::RegisterComponent(ActorComponent* pCom)
{
	components.insert(pCom);
}

void Actor::UnregisterComponent(ActorComponent* pCom)
{
	if (components.find(pCom) != components.end())
		components_iter = components.erase(components.find(pCom));
}


void Actor::Destroy()
{
	if (parent)parent->children.erase(this);

	std::stack<Actor*>objects_to_delete;
	objects_to_delete.push(this);
	this->EndPlay();

	while (!objects_to_delete.empty()) {
		Actor* current_object = objects_to_delete.top();
		objects_to_delete.pop();

		if (!current_object->children.empty())
		{
			for (auto& child : current_object->children)
			{
				objects_to_delete.push(child);
			}
		}
		mainWorld.GameActors_to_delete.insert(current_object);
		current_object->EndPlay();
	}
}

const Vector2D& Actor::GetLocalPosition() const
{
	return root->GetLocalPosition();
}

float Actor::GetLocalRotation() const
{
	return root->GetLocalRotation();
}

const Vector2D& Actor::GetLocalScale() const
{
	return root->GetLocalScale();
}


Vector2D Actor::GetWorldPosition() const
{
	if (parent)return parent->GetWorldPosition() + GetLocalPosition();
	else return GetLocalPosition();
}

float Actor::GetWorldRotation() const
{
	if (parent)return parent->GetWorldRotation() + GetLocalRotation();
	else return GetLocalRotation();
}

Vector2D Actor::GetWorldScale() const
{
	if (parent)return parent->GetWorldScale() * GetLocalScale();
	else return GetLocalScale();
}

void Actor::SetLocalPosition(const Vector2D& pos)
{
	root->SetLocalPosition(pos);
}

void Actor::SetLocalRotation(float angle)
{
	root->SetLocalRotation(angle);
}

void Actor::SetLocalScale(const Vector2D& scale)
{
	root->SetLocalScale(scale);
}

void Actor::AddPosition(Vector2D pos)
{
	root->AddPosition(pos);
}

void Actor::AddRotation(float rot)
{
	root->AddRotation(rot);
}

void Actor::DrawDebugPosition() const
{
	settextstyle(25, 10, "Arial");
	settextcolor(WHITE);
	Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition())
		* 20.f / mainWorld.mainCamera->springArmLength_virtual + Vector2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
	outtextxy((int)pos.x, (int)pos.y, GetWorldPosition().ToString().c_str());
}