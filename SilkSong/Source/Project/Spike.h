#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Damagable.h"


class Spike :public Actor, public IDamagable
{
public:
	Spike();

	virtual void Update(float deltaTime)override;

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override { return{}; }

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override {}

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	class SpriteRenderer* render;
	class BoxCollider* box;

	Animator* ani;
	Animation ready;
	Animation start;
	Animation idle;
	Animation end;
	AnimEdge start_to_idle;

	AnimationDelegate destroy;

	Timer AttackTimerHandle;
	Timer IdleTimerHandle;
};
