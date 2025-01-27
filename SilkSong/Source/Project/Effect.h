#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class Effect :public Actor
{
public:
	Effect();

	void Init(std::string path, float delta = 0);

protected:
	class SpriteRenderer* render;
	Animator* ani;

	Animation effect;
	AnimationDelegate destroy;
};