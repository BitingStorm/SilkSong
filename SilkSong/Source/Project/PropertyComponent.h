#pragma once
#include "Components/ActorComponent.h"



class PropertyComponent :public ActorComponent
{
protected:
	int32 health;

	int32 maxHealth;

public:
	PropertyComponent();

	int32 GetHealth()const { return health; }

	int32 AddHealth(int32 delta);

	int32 GetMaxHealth()const { return maxHealth; }

	void SetMaxHealth(int32 max) { maxHealth = max; }
};