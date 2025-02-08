#pragma once
#include "Objects/Actor.h"



class GameMode :public Actor
{
public:
	GameMode();

protected:
	class DamageSystem* damageSystem;
};