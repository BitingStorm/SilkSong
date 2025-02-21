#pragma once
#include "Components/ActorComponent.h"
#include "Damagable.h"



class DamageSystem :public ActorComponent
{
public:
	DamageSystem();

	void ApplyDamage(IDamagable* damageCauser, IDamagable* damagedActor, float baseValue, EDamageType damageType);
};