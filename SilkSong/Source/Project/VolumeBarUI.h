#pragma once
#include "UI/UserInterface.h"


class VolumeBarUI :public UserInterface
{
	Text* VolumeText;
	Bar* VolumeBar;
	Button* VolumeButton;
	Text* VolumeNumber;

	int32 index;
public:
	VolumeBarUI();

	void Init(int32 i);

	virtual void Update(float deltaTime)override;
};
