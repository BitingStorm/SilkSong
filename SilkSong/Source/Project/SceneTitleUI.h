#pragma once
#include "UI/UserInterface.h"
#include "Components/Animator.h"

class SceneTitleUI :public UserInterface
{
	Text* Title;
	Image* Top;
	Image* Bottom;
	Animation topStart;
	Animation bottomStart;
	Animation topEnd;
	Animation bottomEnd;

	float timer;
	float step;
	Timer DestroyTimerHandle;
public:
	SceneTitleUI();

	virtual void Update(float deltaTime)override;
};