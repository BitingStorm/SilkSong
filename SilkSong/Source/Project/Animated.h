#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class Animated :public Actor
{
public:
	Animated();

	void Init(std::string name, float delay, int layer);

protected:
	class SpriteRenderer* render;
	Animator* ani;

	Animation idle;
};
