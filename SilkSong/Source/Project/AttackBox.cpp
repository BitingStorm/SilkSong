#include "AttackBox.h"
#include "Components/Collider.h"
#include "Enemy.h"
#include "GameplayStatics.h"
#include "Effect.h"
#include "Dart.h"
#include "Tools/Math.h"



AttackBox::AttackBox()
{
	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetSize({ 200,150 });
	box->SetType(CollisionType::HurtBox);

	DestroyTimerHandle.Bind(0.05f, [this]() {Destroy(); }, false);

	box->OnComponentBeginOverlap.AddDynamic(this, &AttackBox::OnOverlap);
	box->OnComponentEndOverlap.AddDynamic(this, &AttackBox::OnEndOverlap);
}

void AttackBox::Init(ECharacterDirection direction)
{
	this->direction = direction;
	if (direction == ECharacterDirection::LookUp)
	{
		box->SetSize({ 100, 200 });
		box->AddPosition({ 0, -50 });
	}
	else if (direction == ECharacterDirection::LookDown)
	{
		box->SetSize({ 100, 125 });
		box->AddPosition({ 0, 50 });
	}
}


void AttackBox::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	Vector2D normal = (-GetOwner()->GetWorldPosition() + otherActor->GetWorldPosition()).Normalize();
	if (Enemy* enemy = Cast<Enemy>(otherActor))
	{
		if (!GetOwner())return;
		enemy->TakeDamage(normal);
		this->enemy = enemy;
		if (direction == ECharacterDirection::LookDown)Cast<Player>(GetOwner())->Bounce();
		if (Math::RandInt(0, 10) > 5)GameplayStatics::PlaySound2D("sound_damage_0");
		else GameplayStatics::PlaySound2D("sound_damage_1");
	}
	else if (Cast<Dart>(otherActor))
	{
		if (direction == ECharacterDirection::LookDown)Cast<Player>(GetOwner())->Bounce();
		GameplayStatics::PlaySound2D("sound_swordhit");
		GameplayStatics::PlayCameraShake(3);
		Effect* effect = GameplayStatics::CreateObject<Effect>(otherActor->GetWorldPosition());
		if (!effect)return;
		effect->Init("effect_nailhit");
		effect->SetLocalRotation(Vector2D::VectorToDegree(normal) + 10);
	}
}

void AttackBox::OnEndOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (hitComp->GetType() == CollisionType::Enemy || hitComp->GetType() == CollisionType::Dart)
	{
		box->SetCollisonMode(CollisionMode::None);
	}
}

void AttackBox::EndPlay()
{
	Super::EndPlay();

	if (enemy && enemy->IsDead())
	{
		Effect* effect = GameplayStatics::CreateObject<Effect>(enemy->GetWorldPosition());
		if (!effect)return;
		effect->Init("effect_death");
	}
}
