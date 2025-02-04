#pragma once
#include "Objects/Actor.h"


class Bg :public Actor
{
public:
	Bg();

	virtual void Update(float deltaTime)override;

	void Init(std::string path, int layer = -1, bool blur = false, bool adjust = true);

protected:
	class SpriteRenderer* render;

	FVector2D InitPos;

	class Player* player;

	bool bAdjust;
};