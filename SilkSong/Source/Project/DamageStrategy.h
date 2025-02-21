#pragma once
#include "Damagable.h"


class DamageStrategy
{
protected:
	class Actor* Owner;
public:
	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
	{
		return FDamageCauseInfo(true, damageCauser, baseValue);
	}

	void SetOwner(Actor* owner)
	{
		Owner = owner;
	}
};