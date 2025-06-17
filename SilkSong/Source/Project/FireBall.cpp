#include "FireBall.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/SpriteRenderer.h"
#include "GameplayStatics.h"
#include "Components/ParticleSystem.h"
#include "Effect.h"
#include "Player.h"
#include "GameModeHelper.h"


FireBall::FireBall()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->SetLayer(3);

	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	idle.Load("effect_fireball");
	idle.SetInterval(0.08f);
	ani->Insert("idle", idle);
	ani->SetNode("idle");

	rigid = ConstructComponent<RigidBody>();
	rigid->SetLinearDrag(0.5f);
	rigid->SetGravityEnabled(false);

	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetCollisonMode(CollisionMode::Collision);
	circle->SetType(CollisionType::Bullet);
	circle->SetRadius(16);

	circle->OnComponentHit.AddDynamic(this, &FireBall::OnHit);

	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetCapacity(15);
	particle->SetInterval(0.02);
	particle->Load("particle_grimm_smoke");
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(30, 0, 10);
	particle->SetFadingInTime(0.1f);
	particle->SetFadingOutTime(0.5f);
	particle->SetLifeCycle(0.6f);
	particle->SetSizeRange(0.15f, 0.4f);
	particle->SetLayer(3);

	SpawnTimerHandle.Bind(0.05f, [this]() {
		GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D::DegreeToVector(FMath::RandInt(0, 360)) * 25,
		0, FVector2D::UnitVector * FMath::RandReal(0.6f, 1.2f))->Init("effect_flame_particle", 0.01f);
		}, true);
}

void FireBall::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	rigid->AddImpulse(FVector2D(deltaTime * direction * 600.f, 0));

	if (bShrinking)
	{
		SetLocalScale(GetLocalScale() * 0.95f);
		if (GetLocalScale().x < 0.1f)
		{
			Destroy();
		}
	}
}

void FireBall::Init(FVector2D impulse)
{
	rigid->AddImpulse(impulse);
	if (FMath::Abs(impulse.x) < 1)direction = 0;
	else direction = impulse.x > 0 ? 1 : -1;
}

void FireBall::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, player, 1, EDamageType::Enemy);
	}
	GameplayStatics::PlayCameraShake(4, 10);
	particle->SetIsLoop(false);
	bShrinking = true;
	circle->SetCollisonMode(CollisionMode::None);
}
