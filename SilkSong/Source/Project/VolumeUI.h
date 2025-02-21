#pragma once
#include "UI/UserInterface.h"


class VolumeUI :public UserInterface
{
	Text* Title;

	Button* Back;
	Text* BackText;

	class VolumeBarUI* volumeBarUI[2];

	class MenuUI* menuUI;

public:
	VolumeUI();

	virtual void Update(float deltaTime)override;

	void SetOwner(MenuUI* owner) { menuUI = owner; }
};