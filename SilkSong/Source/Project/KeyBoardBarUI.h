#pragma once
#include "UI/UserInterface.h"


class KeyBoardBarUI :public UserInterface
{
	Text* ActionText;
	int32 index;
	Button* ChangeButton;
	class ButtonIconUI* Icon;
	Text* SettingText;
	VerticalPanel* Panel;

public:
	KeyBoardBarUI();
	void Init(int i);
	int32 GetIndex()const { return index; }
	bool SetIcon(uint8 keyCode);
};