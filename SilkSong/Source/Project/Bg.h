#pragma once
#include "Objects/Actor.h"


class Bg :public Actor
{
public:
	Bg();

	virtual void Update(float deltaTime)override;

	void Init(std::string path, int layer = -1, int moveLayer = 0, bool blur = false, float k = 1.05);

protected:
	class SpriteRenderer* render;

	FVector2D InitPos;

	class Player* player;

	int32 moveLayer;

	float k;
};