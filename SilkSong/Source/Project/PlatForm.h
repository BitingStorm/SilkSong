#pragma once
#include "Objects/Actor.h"


class PlatForm :public Actor
{
public:
	PlatForm();

	void Init(std::string path, Vector2D size, Vector2D delta);
protected:
	class SpriteRenderer* render;
	class BoxCollider* box;
};