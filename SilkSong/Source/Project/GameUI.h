#pragma once
#include "UI/UserInterface.h"



class GameUI :public UserInterface
{
	Image* SoulContainer;

	Image* SoulContainer_;

	Image* Health[5];

	Image* Health_[5];

	Image* Coin;

	Text* CoinNum;

	Image* Dart;

	Text* DartNum;

public:
	GameUI();

	virtual void Update(float deltaTime)override;
};