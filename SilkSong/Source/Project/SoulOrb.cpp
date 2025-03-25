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
	rigid->SetLinearDrag(0.7f);
	rigid->SetGravityEnabled(false);

	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetCollisonMode(CollisionMode::Collision);
	circle->SetType(CollisionType::Bullet);
	circle->SetRadius(30);

	circle->OnComponentHit.AddDynamic(this, &SoulOrb::OnHit);

	SpawnTimerHandle.Bind(0.08f, [this]() {
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D::DegreeToVector(FMath::RandInt(0, 360)) * 30);
		effect->Init("effect_soulorb", FMath::RandReal(-0.01f, 0.01f));
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.f));
		effect->SetLocalRotation(FMath::RandInt(0, 360));
		
		if (moveLock && !GetOwner())
		{
			moveLock = false;
			Effect* effect_ = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
			effect_->Init("effect_soulspawn"); effect_->SetLocalScale(FVector2D(1, 1) * 1.5f);
			render->Activate();
		}
		}, true, 0.4f);

	moveLock = true;

	SetLocalScale(FVector2D::ZeroVector);

	render->Deactivate();
}

void SoulOrb::BeginPlay()
{
	Actor::BeginPlay();

	player = Cast<Player>(GameplayStatics::GetController());

	if (GetOwner())
	{
		render->Activate();
		return;
	}

	EndTimerHandle.Bind(3.5f, [this]() {OnHit(nullptr, nullptr, nullptr, FVector2D::UnitVector, {});});

	for (int i = 0; i < 10; i++)
	{
		FVector2D unit = FVector2D::DegreeToVector(FMath::RandReal(0, 360));
		float real = FMath::RandReal(100.f, 300.f);
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + unit * real);
		effect->Init("effect_soulorb", -0.02f + real * 0.0001f, -unit * real * 1.2f);
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.f));
		effect->SetLocalRotation(FMath::RandReal(0, 360));
	}
}

void SoulOrb::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (GetOwner())
	{
		return;
	}

	if (player && !moveLock)
	{
		rigid->AddImpulse((player->GetWorldPosition() - GetWorldPosition()).GetSafeNormal() * deltaTime * 1400.f);
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
	effect->GetComponentByClass<SpriteRenderer>()->SetLayer(1);
	Destroy();

	for (int i = 0; i < 20; i++)
	{
		FVector2D unit = FVector2D::DegreeToVector(FMath::RandReal(0, 360));
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + unit * FMath::RandReal(0, 100));
		effect->Init("effect_soulorb", FMath::RandReal(-0.01f, 0.01f), FMath::RandReal(150, 400) * unit);
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.2f, 0.8f));
		effect->SetLocalRotation(FMath::RandReal(0, 360));
	}
}
