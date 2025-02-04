#pragma once
#include "Objects/Actor.h"


class PlatForm :public Actor
{
public:
	PlatForm();

	void Init(std::string path, FVector2D size, FVector2D delta);
protected:
	class SpriteRenderer* render;
	class BoxCollider* box;
};