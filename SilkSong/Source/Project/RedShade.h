#pragma once
#include "Objects/Actor.h"


class RedShade :public Actor
{
public:
	RedShade();

	virtual void Update(float deltaTime)override;
private:
	class SpriteRenderer* render;
	float alpha = 255.f;
};