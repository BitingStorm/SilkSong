#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class Pointer :public Actor
{
public:
	Pointer();

	void FadeOut();
protected:
	class SpriteRenderer* render;
	Animator* ani;

	Animation effect;
	AnimationDelegate destroy;
};