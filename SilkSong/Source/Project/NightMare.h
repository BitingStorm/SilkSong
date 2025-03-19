#pragma once
#include "Enemy.h"


class NightMare :public Enemy
{
	DEFINE_SUPER(Enemy)

public:
	NightMare();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);

	virtual void SpawnGeos()override;

	virtual void Die()override;

private:
	Animation idle;
	Animation bow;
};
