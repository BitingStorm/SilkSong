#pragma once
#include "Objects/Actor.h"


class ParticleSystem;

class NewTheme :public Actor
{
	DEFINE_SUPER(Actor)

	ParticleSystem* particles[4];

public:
    NewTheme();
};