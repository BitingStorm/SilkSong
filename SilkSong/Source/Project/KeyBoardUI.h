#pragma once
#include "RichTextUI.h"
#include "Components/Animator.h"


class KeyBoardUI :public UserInterface
{
	RichTextUI* Title;
	Image* Warning;
	Animation idle;

	Button* Back;
	RichTextUI* BackText;

	GridPanel* KeyBoardPanel;
	class KeyBoardBarUI* currentBar;
public:
	KeyBoardUI();

	virtual void Update(float deltaTime)override;

	void SetCurrentBar(KeyBoardBarUI* cur) { currentBar = cur; }
};