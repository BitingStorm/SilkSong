#pragma once
#include "CoreMinimal.h"

enum class EDamageType :uint8
{
	Player,
	Enemy,
	Other
};

class IDamagable;

struct FDamageCauseInfo
{
	bool bIsValid;
	IDamagable* damageCauser;
	float realValue;

	FDamageCauseInfo()
		:bIsValid(false), damageCauser(nullptr), realValue(0.f) {}

	FDamageCauseInfo(bool isValid, IDamagable* causer, float value)
		:bIsValid(isValid), damageCauser(causer), realValue(value) {}
};

class IDamagable
{
public:
	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType) = 0;

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo) = 0;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo) = 0;
};

DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(OnDamageDealt, FDamageCauseInfo)
DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(OnDamageTaken, FDamageCauseInfo)