#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class Effect :public Actor
{
public:
	Effect();

	void Init(std::string path, float delta = 0, FVector2D vel = FVector2D::ZeroVector);

	virtual void Update(float deltaTime)override;

protected:
	class SpriteRenderer* render;
	Animator* ani;

	Animation effect;
	AnimationDelegate destroy;

	FVector2D velocity;
};