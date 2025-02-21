#include "DamageSystem.h"


DamageSystem::DamageSystem()
{
	
}

void DamageSystem::ApplyDamage(IDamagable* damageCauser, IDamagable* damagedActor, float baseValue, EDamageType damageType)
{
	CHECK_PTR(damageCauser)
	CHECK_PTR(damagedActor)
	FDamageCauseInfo causeInfo = damagedActor->TakeDamage(damageCauser, baseValue, damageType);
	damagedActor->ExecuteDamageTakenEvent(causeInfo);
	damageCauser->ExecuteDamageDealtEvent(causeInfo);
}