#pragma once
#include "RichTextUI.h"


class VolumeBarUI :public UserInterface
{
	RichTextUI* VolumeText;
	Bar* VolumeBar;
	Button* VolumeButton;
	Text* VolumeNumber;

	int32 index;
public:
	VolumeBarUI();

	void Init(int32 i);

	virtual void Update(float deltaTime)override;
};
