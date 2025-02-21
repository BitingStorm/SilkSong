#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Damagable.h"


class CloseSkillBox :public Actor, public IDamagable
{
public:
	CloseSkillBox();

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override { return{}; }

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override {}

private:
	class CircleCollider* circle;
	Timer DestroyTimerHandle;
	Timer AttackTimerHandle;

	class SpriteRenderer* render;
	Animator* ani;
	Animation effect;
};