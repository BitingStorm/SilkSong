#pragma once
#include "Objects/Actor.h"



class RainProducer :public Actor
{
	DEFINE_SUPER(Actor)

public:
	RainProducer();
private:
	class ParticleSystem* particle;
	class ParticleSystem* particle_;
};