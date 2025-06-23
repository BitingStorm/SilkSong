#pragma once
#include "Objects/Actor.h"
#include "Damagable.h"


class Needle :public Actor, public IDamagable
{
public:
	Needle();

	virtual void Update(float deltaTime)override;

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override { return{}; }

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override {}

	void Init(bool rapid);

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	class SpriteRenderer* render;
	class BoxCollider* box;

	float absDistance;
	float distance;
	float direction;
	bool bIsRapid;
};