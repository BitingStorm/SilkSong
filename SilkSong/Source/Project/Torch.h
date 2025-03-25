#pragma once
#include "Objects/Actor.h"


class Torch :public Actor
{
public:
	Torch();

private:
	class ParticleSystem* particle;
	class SpriteRenderer* render;
};
