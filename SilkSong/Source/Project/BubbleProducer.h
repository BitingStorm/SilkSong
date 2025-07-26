#pragma once
#include "Objects/Actor.h"



class BubbleProducer :public Actor
{
	DEFINE_SUPER(Actor)

public:
	BubbleProducer();
private:
	class ParticleSystem* particle;
};
