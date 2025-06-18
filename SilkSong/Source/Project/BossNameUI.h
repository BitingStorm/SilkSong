#pragma once
#include "UI/UserInterface.h"


class BossNameUI :public UserInterface
{
	Text* text_;
	Text* text;

	int32 flag = 1;
	float alpha = 0;
	Timer FadingTimerHandle;
public:
	BossNameUI();

	virtual void Update(float deltaTime)override;

	void InitName(std::string name, std::string name_);
};
