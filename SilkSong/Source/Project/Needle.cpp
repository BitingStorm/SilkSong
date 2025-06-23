#include "Needle.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Enemy.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"
#include "Chest.h"


Needle::Needle()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("needle");
	render->SetLayer(5);

	box = ConstructComponent<BoxCollider>();
	box->SetType(CollisionType::HurtBox);
	box->SetSize({ 150,20 });
	box->AttachTo(root);

	box->OnComponentBeginOverlap.AddDynamic(this, &Needle::OnOverlap);

	distance = 0;
	direction = 0;
	absDistance = 0;
	bIsRapid = false;
}

void Needle::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (direction == 0)
	{
		direction = GetLocalScale().x;
	}

	float delta = (bIsRapid ? 5000 : 3000) * deltaTime * direction;
	AddPosition({ delta, 0 });
	distance += delta;
	absDistance += FMath::Abs(delta);

	if (FMath::Abs(distance) > (bIsRapid ? 300 : 750))
	{
		distance = (bIsRapid ? 300 : 750) * direction;
		if (bIsRapid)
		{
			box->SetCollisonMode(CollisionMode::None);
		}
		else
		{
			SetLocalScale({ -1,1 });
		}
		direction = -direction;
	}

	if (absDistance > (bIsRapid ? 600 : 1500))
	{
		Destroy();
	}
}

void Needle::Init(bool rapid)
{
	bIsRapid = rapid; if (rapid)box->SetSize({ 150,100 });
}

void Needle::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Enemy* enemy = Cast<Enemy>(otherActor))
	{
		if (enemy->IsDead())
		{
			return;
		}
		GameModeHelper::ApplyDamage(this, enemy, bIsRapid ? 8 : 5, EDamageType::Player);
		GameModeHelper::PlayFXSound("sound_damage_1");
	}
	else if (Chest* chest = Cast<Chest>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, chest, 1, EDamageType::Player);
	}
}