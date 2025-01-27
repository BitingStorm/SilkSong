#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class CloseSkillBox :public Actor
{
public:
	CloseSkillBox();

private:
	class CircleCollider* circle;
	Timer DestroyTimerHandle;
	Timer AttackTimerHandle;

	class SpriteRenderer* render;
	Animator* ani;
	Animation effect;
};