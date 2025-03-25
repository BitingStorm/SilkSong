#pragma once
#include "Objects/Actor.h"
#include "Components/Animator.h"


class Brumm :public Actor
{
public:
	Brumm();

protected:
	class SpriteRenderer* render;
	Animator* ani;
	Animation idle;
	class AudioPlayer* audio;
};
