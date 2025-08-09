#include "Bone.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "GameModeHelper.h"


Bone::Bone()
{
	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Block);

	box->OnComponentBeginOverlap.AddDynamic(this, &Bone::OverlapBegin);
	box->OnComponentEndOverlap.AddDynamic(this, &Bone::OverlapEnd);
}

void Bone::Update(float deltaTime)
{
	Bg::Update(deltaTime);

	render->SetLocalPosition(FMath::Lerp(render->GetLocalPosition(), aim, 0.1f));
}

void Bone::Init_(FVector2D size, FVector2D delta)
{
	box->SetSize(size);
	box->SetLocalPosition(delta);
}

void Bone::OverlapBegin(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (otherComp->GetType() == CollisionType::Player && hitComp->GetWorldPosition().y > otherComp->GetLocalPosition().y)
	{
		aim = FVector2D(0, 5.f);
		GameModeHelper::PlayFXSound("sound_bonewalk_" + std::to_string(FMath::RandInt(0, 2)));
	}
}

void Bone::OverlapEnd(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (otherComp->GetType() == CollisionType::Player)
	{
		aim = FVector2D::ZeroVector;
	}
}

