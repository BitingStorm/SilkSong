#pragma once
#include "Enemy.h"
#include "Tools/Timer.h"


class Bug :public Enemy
{
	DEFINE_SUPER(Enemy)

public:
	Bug();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const struct HitResult& hitResult);

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
	AnimEdge walk_to_die;

	class BoxCollider* currentPlatForm;
	
	Timer StateUpdateTimerHandle;
	bool bIsBuried = false;
};
