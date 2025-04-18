#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Damagable.h"


class SoulOrb :public Actor, public IDamagable
{
public:
	SoulOrb();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override { return{}; }

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override {}

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);

	class SpriteRenderer* render;
	class CircleCollider* circle;
	class RigidBody* rigid;
	Animator* ani;
	Animation idle;
	Timer SpawnTimerHandle;
	Timer EndTimerHandle;

	class Player* player;
	bool moveLock;
};