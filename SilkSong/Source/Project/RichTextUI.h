#pragma once
#include "UI/UserInterface.h"


class RichTextUI :public UserInterface
{
	Text* First;
	Text* Others;
public:
	RichTextUI();
	void SetText(std::string text, int size);
	void SetAlpha(BYTE alpha);
	void SetPosition(FVector2D pos);
};
