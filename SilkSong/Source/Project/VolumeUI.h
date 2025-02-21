#pragma once
#include "UI/UserInterface.h"
#include "Components/Animator.h"


class VolumeUI :public UserInterface
{
	Text* Title;
	Image* Warning;
	Animation idle;

	Button* Back;
	Text* BackText;

	class VolumeBarUI* volumeBarUI[2];

	class MenuUI* menuUI;

public:
	VolumeUI();

	virtual void Update(float deltaTime)override;

	void SetOwner(MenuUI* owner) { menuUI = owner; }
};