#pragma once
#include "RichTextUI.h"

class Pointer;

class MenuUI :public UserInterface
{
	Image* Title;
	Image* Logo;
	Image* Icon;
	Image* Black;

	Text* Edition;
	RichTextUI* Options[5];
	Button* Buttons[5];

	int32 startFlag = 0;

	class VolumeUI* volumeUI;
	class KeyBoardUI* keyboardUI;
public:
	MenuUI();

	virtual void Update(float deltaTime)override;
};