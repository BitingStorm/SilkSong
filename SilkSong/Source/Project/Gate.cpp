#include "Gate.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"


Gate::Gate()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("gate");

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Block);
	box->SetSize({ 50,250 });
}

void Gate::Update(float deltaTime)
{
	Actor::Update(deltaTime);

}

void Gate::Open()
{
	box->SetCollisonMode(CollisionMode::None);
	AddPosition({ 0,-200 });
}

void Gate::Close()
{
	box->SetCollisonMode(CollisionMode::Collision);
	AddPosition({ 0,200 });
}
