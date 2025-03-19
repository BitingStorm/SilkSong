#pragma once
#include "Enemy.h"


class Bug :public Enemy
{
	DEFINE_SUPER(Enemy)

public:
	Bug();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const struct FHitResult& hitResult);

	virtual void SpawnGeos()override;

	virtual void Die()override;

private:
	Animation walk;
	Animation turn;
	Animation bury;
	Animation appear;
	Animation die;
	AnimEdge turn_to_walk;
	AnimEdge appear_to_walk;
	AnimEdge walk_to_bury;
	AnimEdge bury_to_appear;

	class BoxCollider* currentPlatForm;
	
	Timer StateUpdateTimerHandle;
	bool bIsBuried = false;
};