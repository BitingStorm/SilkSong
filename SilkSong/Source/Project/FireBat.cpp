#include "FireBat.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/SpriteRenderer.h"
#include "Player.h"
#include "GameModeHelper.h"
#include "GameplayStatics.h"
#include "Components/ParticleSystem.h"
#include "Effect.h"


FireBat::FireBat()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->SetLayer(3);

	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	idle.Load("firebat_idle");
	idle.SetInterval(0.08f);
	destroy.Load("firebat_destroy");
	destroy.SetInterval(0.08f);
	destroy.OnAnimExit.Bind([this]() {Destroy(); });
	ani->Insert("idle", idle);
	ani->Insert("destroy", destroy);
	ani->SetNode("idle");

	rigid = ConstructComponent<RigidBody>();
	rigid->SetLinearDrag(0);
	rigid->SetGravityEnabled(false);

	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetCollisonMode(CollisionMode::Collision);
	circle->SetType(CollisionType::Bullet);
	circle->SetRadius(30);

	circle->OnComponentHit.AddDynamic(this, &FireBat::OnHit);

	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetCapacity(15);
	particle->SetInterval(0.02);
	particle->Load("particle_grimm_smoke");
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 0,30 }, { 0,360 });
	particle->SetFadingInTime(0.1f);
	particle->SetFadingOutTime(0.5f);
	particle->SetLifeCycle(0.6f);
	particle->SetSizeRange(0.2f,0.5f);
	particle->SetLayer(3);

	SpawnTimerHandle.Bind(0.02f, [this]() {
		GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D::DegreeToVector(FMath::RandInt(0, 360)) * 25,
		0, FVector2D::UnitVector * FMath::RandReal(0.6f, 1.2f))->Init("effect_flame_particle", 0.01f);
		}, true);
}

void FireBat::BeginPlay()
{
	Actor::BeginPlay();

	rigid->AddImpulse(FVector2D(GetWorldScale().x * 1000.f, 0));
	GameModeHelper::PlayFXSound("sound_fireball_cast");

	GameplayStatics::CreateObject<Effect>(GetWorldPosition())->Init("effect_nightmare_cast", -0.02f);
	player = Cast<Player>(GameplayStatics::GetController());
}

void FireBat::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (player)
	{
		rigid->AddImpulse(FVector2D(0, (player->GetWorldPosition().y - GetWorldPosition().y) > 0 ? 1 : -1) * deltaTime * 200);
	}
}

void FireBat::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, player, 1, EDamageType::Enemy);
	}
	GameplayStatics::PlayCameraShake(5, 5);
	ani->PlayMontage("destroy");
	particle->SetIsLoop(false);
}
