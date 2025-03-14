#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class RoarEffect :public Actor
{
public:
	RoarEffect();

	virtual void Update(float deltaTime)override;

protected:
	class SpriteRenderer* render;
};
