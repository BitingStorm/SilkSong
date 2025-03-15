#include "SoulOrb.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Components/RigidBody.h"
#include "Components/ParticleSystem.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"
#include "Player.h"
#include "Effect.h"


SoulOrb::SoulOrb()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->SetLayer(1);
	render->LoadSprite("soul_orb");

	rigid = ConstructComponent<RigidBody>();
	rigid->SetLinearDrag(0.5f);
	rigid->SetGravityEnabled(false);

	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetCollisonMode(CollisionMode::Collision);
	circle->SetType(CollisionType::Bullet);
	circle->SetPhysicsMaterial(FPhysicsMaterial(0.5f, 0.5f));
	circle->SetRadius(30);

	circle->OnComponentHit.AddDynamic(this, &SoulOrb::OnHit);

	SpawnTimerHandle.Bind(0.08f, [this]() {
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D::DegreeToVector(FMath::RandInt(0, 360)) * 30);
		effect->Init("effect_soulorb", -0.01f);
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.f));
		effect->SetLocalRotation(FMath::RandInt(0, 360));
		}, true);
}

void SoulOrb::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (GetOwner())
	{
		return;
	}

	if (!player)
	{
		player = Cast<Player>(GameplayStatics::GetController());
		for (int i = 0; i < 10; i++)
		{
			FVector2D unit = FVector2D::DegreeToVector(FMath::RandReal(0, 360));
			float real = FMath::RandReal(100, 300);
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + unit * real);
			effect->Init("effect_soulorb", -0.01f, -unit * real);
			effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.f));
			effect->SetLocalRotation(FMath::RandReal(0, 360));
		}
	}

	if (player)
	{
		rigid->AddImpulse((player->GetWorldPosition() - GetWorldPosition()).GetSafeNormal() * deltaTime * 1000.f);
	}
}

void SoulOrb::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (Player* player = Cast<Player>(otherActor))
	{	
		GameModeHelper::ApplyDamage(this, player, 1, EDamageType::Enemy);
	}
	GameplayStatics::PlayCameraShake(7, 5);
	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	effect->Init("effect_soulburst");
	effect->SetLocalScale(FVector2D(1, 1) + normalImpulse.GetAbs());
	Destroy();

	for (int i = 0; i < 20; i++)
	{
		FVector2D unit = FVector2D::DegreeToVector(FMath::RandReal(0, 360));
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + unit * FMath::RandReal(0, 200));
		effect->Init("effect_soulorb", -0.01f, FMath::RandReal(100, 300) * unit);
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.2f, 0.8f));
		effect->SetLocalRotation(FMath::RandReal(0, 360));
	}
}
