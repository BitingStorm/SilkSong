#pragma once
#include "Objects/Actor.h"



class Dart :public Actor
{
public:
	Dart();

	void Init(bool bIsLeft);

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, Vector2D normalImpulse, const struct HitResult& hitResult);

	class SpriteRenderer* render;
	class CircleCollider* circle;
	class RigidBody* rigid;
};