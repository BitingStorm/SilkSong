#pragma once
#include "Objects/Actor.h"



class Geo :public Actor
{
public:
	Geo();

	void Init(std::string name, int price);

	virtual void Update(float deltaTime)override;

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);

	class SpriteRenderer* render;
	class BoxCollider* box;
	class RigidBody* rigid;

	int price;
	
	float rotateDelta;
};