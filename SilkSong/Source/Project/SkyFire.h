#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Damagable.h"


class SkyFire :public Actor, public IDamagable
{
public:
	SkyFire();

	virtual void Update(float deltaTime)override;

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override { return{}; }

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override {}

	void Init(FVector2D impulse);

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	class CircleCollider* circle;
	class RigidBody* rigid;
	class ParticleSystem* particle;
	class ParticleSystem* particle_;
};
