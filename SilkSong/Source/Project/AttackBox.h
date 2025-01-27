#pragma once
#include "Player.h"
#include "Tools/Timer.h"



class AttackBox :public Actor
{
	DEFINE_SUPER(Actor)

public:
	AttackBox();

	void Init(ECharacterDirection direction);

	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	void OnEndOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	virtual void EndPlay()override;
private:
	class BoxCollider* box;
	Timer DestroyTimerHandle;
	class Enemy* enemy = nullptr;
	ECharacterDirection direction;
};