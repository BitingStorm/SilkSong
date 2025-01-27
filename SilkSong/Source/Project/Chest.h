#pragma once
#include "Objects/Actor.h"



class Chest :public Actor
{
public:
	Chest();

	void TakeDamage();

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	class SpriteRenderer* render;
	class BoxCollider* box;

	int32 num;
};