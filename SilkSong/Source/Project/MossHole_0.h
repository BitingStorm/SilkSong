#pragma once
#include "Objects/Level.h"


class MossHoleLevel0 :public Level
{
	class Player* player;
	class SceneTitleUI* ui;
public:
	MossHoleLevel0();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;
};
