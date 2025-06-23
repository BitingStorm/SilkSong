#include "AttackBox.h"
#include "Components/Collider.h"
#include "Enemy.h"
#include "GameplayStatics.h"
#include "Effect.h"
#include "Dart.h"
#include "Interactive.h"
#include "Chest.h"
#include "Player.h"
#include "GameModeHelper.h"


AttackBox::AttackBox()
{
	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetSize({ 220,150 });
	box->SetType(CollisionType::HurtBox);
	box->SetCollisonMode(CollisionMode::None);

	AttackTimerHandle.Bind(0.03f, [this]() {box->SetCollisonMode(CollisionMode::Trigger); }, false);
	DestroyTimerHandle.Bind(0.08f, [this]() {Destroy(); }, false);

	box->OnComponentBeginOverlap.AddDynamic(this, &AttackBox::OnOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &AttackBox::OnEndOverlap);
}

void AttackBox::Init(ECharacterDirection direction, int32 damage)
{
	this->direction = direction;
	if (direction == ECharacterDirection::LookUp)
	{
		box->SetSize({ 100, 220 });
		box->AddPosition({ 0, -70 });
	}
	else if (direction == ECharacterDirection::LookDown)
	{
		box->SetSize({ 100, 150 });
		box->AddPosition({ 0, 65 });
	}
	this->damage = damage;
}


void AttackBox::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (!GetOwner())return;
	FVector2D normal = (-GetOwner()->GetWorldPosition() + otherActor->GetWorldPosition()).GetSafeNormal();
	if (Enemy* enemy = Cast<Enemy>(otherActor))
	{
		if (enemy->IsDead())
		{
			return;
		}
		if (direction == ECharacterDirection::LookDown)Cast<Player>(GetOwner())->Bounce();
		GameModeHelper::ApplyDamage(this, enemy, this->damage, EDamageType::Player);
		if (FMath::RandInt(0, 10) > 5)GameModeHelper::PlayFXSound("sound_damage_0");
		else GameModeHelper::PlayFXSound("sound_damage_1");
	}
	else if (Cast<Dart>(otherActor))
	{
		if (direction == ECharacterDirection::LookDown)Cast<Player>(GetOwner())->Bounce();
		GameModeHelper::PlayFXSound("sound_swordhit");
		GameplayStatics::PlayCameraShake(3);
		Effect* effect = GameplayStatics::CreateObject<Effect>(otherActor->GetWorldPosition());
		if (effect)
		{
			effect->Init("effect_darthit", -0.02f);
			effect->SetLocalRotation(FVector2D::VectorToDegree(normal) + 100);
		}
	}
	else if (otherComp->GetType() == CollisionType::Chest)
	{
		GameModeHelper::ApplyDamage(this, Cast<IDamagable>(otherActor), 1, EDamageType::Player);
	}
}

void AttackBox::OnEndOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (hitComp->GetType() == CollisionType::Enemy || hitComp->GetType() == CollisionType::Dart)
	{
		box->SetCollisonMode(CollisionMode::None);
	}
}