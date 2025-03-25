#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Damagable.h"
#include "PropertyCarrier.h"


class Enemy :public Actor, public IDamagable, public IPropertyCarrier
{
	DEFINE_SUPER(Actor)

public:
	Enemy();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override;

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

	virtual PropertyComponent* GetProperty()override;

	bool IsDead()const { return bIsDead; }

	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

protected:
	virtual void Die();

	virtual void SpawnGeos() = 0;

	class SpriteRenderer* render;
	class SpriteRenderer* render_death;
	class CircleCollider* circle;
	class RigidBody* rigid;
	Animator* ani;
	class DamageResponseComponent* damageResponse;
	class PropertyComponent* property;

	class Player* player;

	bool bIsDead = false;
	float hurtTimer;
};