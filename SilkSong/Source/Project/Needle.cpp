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
}

void Needle::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	float delta = 20 * GetWorldScale().x * deltaTime * 100;
	AddPosition({ delta, 0 });
	distance += delta;

	if (FMath::Abs(distance) > 500)
	{
		distance = 500 * GetWorldScale().x;
		SetLocalScale({ -GetLocalScale().x,1 });
	}
}

void Needle::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Enemy* enemy = Cast<Enemy>(otherActor))
	{
		if (enemy->IsDead())
		{
			return;
		}
		GameModeHelper::ApplyDamage(this, enemy, 3, EDamageType::Player);
		GameModeHelper::PlayFXSound("sound_damage_1");
	}
	else if (Chest* chest = Cast<Chest>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, chest, 1, EDamageType::Player);
	}
}