#pragma once
#include "Objects/Actor.h"



class Needle :public Actor
{
public:
	Needle();

	virtual void Update(float deltaTime)override;

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	class SpriteRenderer* render;
	class BoxCollider* box;

	float distance;
};