#pragma once
#include "Objects/Actor.h"
#include "Damagable.h"


class Wave :public Actor, public IDamagable
{
public:
	Wave();

	virtual void Update(float deltaTime)override;

	void Init(int32 index, FVector2D size);

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override { return{}; }

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override {}

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	class SpriteRenderer* render;
	class BoxCollider* box;

	int32 index = 0;
};