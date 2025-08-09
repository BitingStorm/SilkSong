#pragma once
#include "UI/UserInterface.h"


class EscUI :public UserInterface
{
	Image* Black;
	Text* Options[2];
	Button* Buttons[2];
public:
	EscUI();
};
