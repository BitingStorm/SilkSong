#pragma once
#include "Components/ActorComponent.h"
#include "Damagable.h"



class DamageResponseComponent :public ActorComponent
{
	DEFINE_SUPER(ActorComponent)

	class DamageStrategy* strategy;
public:
	DamageResponseComponent();

	virtual void BeginPlay()override;

	FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType);

	void SetStrategy(DamageStrategy* newStrategy);

	virtual void EndPlay()override;
};