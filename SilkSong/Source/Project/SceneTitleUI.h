#pragma once
#include "RichTextUI.h"
#include "Components/Animator.h"

class SceneTitleUI :public UserInterface
{
	RichTextUI* Title_;
	RichTextUI* Title;
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