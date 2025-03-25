#include "Gate.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "GameModeHelper.h"


Gate::Gate()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("gate");
	render->SetLayer(1);

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Block);
	box->SetSize({ 50,250 });
	box->SetCollisonMode(CollisionMode::Collision);
}

void Gate::Update(float deltaTime)
{
	Actor::Update(deltaTime);

}

void Gate::Open()
{
	AddPosition({ 0,-200 });
	GameModeHelper::PlayFXSound("sound_gate_open");
}

void Gate::Close()
{
	AddPosition({ 0,200 });
	GameModeHelper::PlayFXSound("sound_gate_close");
}
