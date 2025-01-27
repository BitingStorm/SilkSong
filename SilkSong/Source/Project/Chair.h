#pragma once
#include "Objects/Actor.h"



class Chair :public Actor
{
public:
	Chair();

protected:
	class SpriteRenderer* render;
	class BoxCollider* box;

	int32 num;
};