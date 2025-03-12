#include "DamageResponseComponent.h"
#include "DefaultDamageStrategy.h"
#include "PropertyCarrier.h"
#include "PropertyComponent.h"



DamageResponseComponent::DamageResponseComponent()
{
	strategy = new DefaultDamageStrategy();
}

void DamageResponseComponent::BeginPlay()
{
	Super::BeginPlay();

	strategy->SetOwner(GetOwner());
}

FDamageCauseInfo DamageResponseComponent::TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
{
	if (!strategy)
	{
		return{};
	}
	FDamageCauseInfo damageInfo = strategy->TakeDamage(damageCauser, baseValue, damageType);
	
	return damageInfo;
}

void DamageResponseComponent::SetStrategy(DamageStrategy* newStrategy)
{
	if (strategy)
	{
		delete strategy;
	}
	strategy = newStrategy;
	strategy->SetOwner(GetOwner());
}

void DamageResponseComponent::EndPlay()
{
	Super::EndPlay();

	delete strategy;
}
