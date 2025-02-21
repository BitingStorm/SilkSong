#pragma once
#include "DamageStrategy.h"


class DefaultDamageStrategy :public DamageStrategy
{
public:
	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override;
};
