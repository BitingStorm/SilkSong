#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class FireWorm :public Actor
{
public:
	FireWorm();
	virtual void Update(float deltaTime)override;
	void Init(int layer = 0);

protected:
	class SpriteRenderer* render;
	Animator* ani;
	Animation idle;

	Timer MoveTimerHandle;
	FVector2D targetPos;
	FVector2D initPos;
	float radius;
};
