#pragma once
#include "Objects/Actor.h"



class Geo :public Actor
{
public:
	Geo();

	void Init(std::string name);

	virtual void Update(float deltaTime)override;

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, Vector2D normalImpulse, const struct HitResult& hitResult);

	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);


	class SpriteRenderer* render;
	class BoxCollider* box;
	class RigidBody* rigid;

	int price;
	
	float rotateDelta;
};