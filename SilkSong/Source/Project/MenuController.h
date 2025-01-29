#pragma once
#include "Objects/Character.h"




class MenuController :public Controller
{
	DEFINE_SUPER(Controller)

	class ParticleSystem* whiteRadiant;
	class AudioPlayer* audio;
	class OldTheme* oldTheme;
	class NewTheme* newTheme;


public:
	MenuController();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	void ChangeTheme();
};