#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"
#include "Tools/Timer.h"

class Dart :public Actor
{
public:
	Dart();

	void Init(bool bIsLeft);

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const struct HitResult& hitResult);

	class SpriteRenderer* render;
	class CircleCollider* circle;
	class RigidBody* rigid;
	Animator* ani;
	Animation idle;
	Timer blinkTimer;
};