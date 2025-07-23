#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Damagable.h"


class Plant :public Actor, public IDamagable
{
public:
	Plant();

	virtual void Update(float deltaTime)override;

	void Init(std::string aniPath, float delay = 0.06f, int layer = -1, int moveLayer = 0);

	void Init_(std::string deathPath, FVector2D boxSize);

	void OnBeginOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override;

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	class SpriteRenderer* render;
	FVector2D InitPos;
	class Player* player;
	int32 moveLayer;

	Animator* ani;
	Animation idle;
	float delay;

	class BoxCollider* box;
	class DamageResponseComponent* damageResponse;
	std::string death;

	Timer ShakeTimerHandle;
};