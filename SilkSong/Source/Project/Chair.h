#pragma once
#include "Objects/Actor.h"



class Chair :public Actor
{
public:
	Chair();
	
	void DisablePointer();

protected:
	class SpriteRenderer* render;
	class SpriteRenderer* pointer;
	class CircleCollider* circle;
	int32 dir = -1;
	int32 num;

	void OnBeginOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);
	void OnEndOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	void Update(float deltaTime);
};