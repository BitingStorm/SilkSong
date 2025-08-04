#pragma once
#include "UI/UserInterface.h"


class ButtonIconUI :public UserInterface
{
protected:
	static std::unordered_map<uint8, std::string>IconStringMap;
	static std::unordered_map<uint8, char>IconCharMap;
	Image* Img;
public:
	ButtonIconUI();
	virtual void SetIcon(uint8 keyCode) = 0;
	static bool IsStringLegal(uint8 keyCode);
	static bool IsCharLegal(uint8 keyCode);
};


class ButtonIconCharUI :public ButtonIconUI
{
	Text* Char;
	bool flag;
public:
	ButtonIconCharUI();
	void SetIcon(uint8 keyCode)override;
	void EnableFlag() { flag = true; }
};

class ButtonIconStringUI :public ButtonIconUI
{
	Text* String;
public:
	ButtonIconStringUI();
	void SetIcon(uint8 keyCode)override;
};

class ButtonIconImageUI :public ButtonIconUI
{
public:
	void SetIcon(uint8 keyCode)override;
};