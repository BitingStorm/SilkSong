#pragma once
#include "Objects/Level.h"


class MossHoleLevel :public Level
{
	class Player* player;
	class SceneTitleUI* ui;
public:
	MossHoleLevel();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;
};