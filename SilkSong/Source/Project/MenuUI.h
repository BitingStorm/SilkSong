#pragma once
#include "UI/UserInterface.h"

class Pointer;

class MenuUI :public UserInterface
{
	Image* Title;
	Image* Logo;
	Image* Icon;
	Image* Black;

	Text* Edition;
	Text* Options[5];
	Button* Buttons[5];

	int32 startFlag = 0;

	class VolumeUI* volumeUI;
public:
	MenuUI();

	virtual void Update(float deltaTime)override;
};