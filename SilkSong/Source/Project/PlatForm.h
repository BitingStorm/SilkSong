#pragma once
#include "Objects/Actor.h"


class PlatForm :public Actor
{
public:
	PlatForm();

	void Init(std::string path, FVector2D size, FVector2D delta = FVector2D::ZeroVector);
protected:
	class SpriteRenderer* render;
	class BoxCollider* box;
};