#pragma once
#include "Objects/Character.h"


class ParticleSystem;

class MenuController :public Controller
{
	DEFINE_SUPER(Controller)


	ParticleSystem* blackRadiantFar;
	ParticleSystem* blackRadiantClose;
	ParticleSystem* whiteRadiant;
	ParticleSystem* smoke;
	class AudioPlayer* audio;

public:
	MenuController();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;
};