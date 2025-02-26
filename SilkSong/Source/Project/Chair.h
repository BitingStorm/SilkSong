#pragma once
#include "Objects/Actor.h"



class Chair :public Actor
{
public:
	Chair();
	
protected:
	class SpriteRenderer* render;
	class CircleCollider* circle;
	class WidgetComponent* widget;
	int32 dir = -1;
	int32 num = 0;

	void OnBeginOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);
	void OnEndOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	void Update(float deltaTime);
};