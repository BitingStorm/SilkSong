#include "MossFloor.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "GameModeHelper.h"


MossFloor::MossFloor()
{
	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Block);

	box->OnComponentBeginOverlap.AddDynamic(this, &MossFloor::OverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &MossFloor::OverlapEnd);

	aim = 1.f;
}

void MossFloor::Update(float deltaTime)
{
	Bg::Update(deltaTime);

	render->SetLocalScale({ 1.f, FMath::Lerp(render->GetLocalScale().y, aim, 0.0875f) });
}

void MossFloor::Init_(FVector2D size, FVector2D delta)
{
	box->SetSize(size);
	box->SetLocalPosition(delta);
}

void MossFloor::OverlapBegin(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (otherComp->GetType() == CollisionType::Player)
	{
		aim = 0.85f;
		GameModeHelper::PlayFXSound("sound_grasswalk_" + std::to_string(FMath::RandInt(0, 2)));
	}
}

void MossFloor::OverlapEnd(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (otherComp->GetType() == CollisionType::Player)
	{
		aim = 1.f;
	}
}