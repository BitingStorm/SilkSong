#include "SkyFire.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/ParticleSystem.h"
#include "Player.h"
#include "GameModeHelper.h"


SkyFire::SkyFire()
{
	rigid = ConstructComponent<RigidBody>();
	rigid->SetGravityEnabled(false);

	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetType(CollisionType::Bullet);
	circle->SetRadius(60);

	circle->OnComponentBeginOverlap.AddDynamic(this, &SkyFire::OnOverlap);

	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetCapacity(10);
	particle->SetInterval(0.02);
	particle->Load("particle_flame_i");
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(10, 0);
	particle->SetFadingOutTime(0.3f);
	particle->SetLifeCycle(0.3f);
	particle->SetLayer(1);

	particle_ = ConstructComponent<ParticleSystem>();
	particle_->AttachTo(root);
	particle_->SetCapacity(10);
	particle_->SetInterval(0.02);
	particle_->Load("particle_flame_o");
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(20, 0, 10, true);
	particle_->SetFadingOutTime(0.3f);
	particle_->SetLifeCycle(0.3f);
	particle_->SetLayer(0);
	particle_->SetSizeRange(0.75, 1.5);
}

void SkyFire::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (GetWorldPosition().y > 1200)
	{
		Destroy();
	}
}

void SkyFire::Init(FVector2D impulse)
{
	rigid->AddImpulse(impulse);
}

void SkyFire::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, player, 1, EDamageType::Enemy);
	}
}
