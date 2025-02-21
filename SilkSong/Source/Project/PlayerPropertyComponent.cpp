#include "PlayerPropertyComponent.h"

PlayerPropertyComponent::PlayerPropertyComponent()
{
	health = 5;
	maxHealth = 5;
	silk = 9;
	geo = 0;
	dartNum = 15;
}

int32 PlayerPropertyComponent::AddSilk(int32 delta, int32 max, int32 min)
{
	int32 initial = silk;
	silk += delta;
    silk = FMath::Clamp(silk, min, max);
	return silk - initial;
}

int32 PlayerPropertyComponent::AddGeo(int32 delta, int32 max, int32 min)
{
	int32 initial = geo;
	geo += delta;
	geo = FMath::Clamp(geo, min, max);
	return geo - initial;
}

int32 PlayerPropertyComponent::AddDart(int32 delta, int32 max, int32 min)
{
	int32 initial = dartNum;
	dartNum += delta;
	dartNum = FMath::Clamp(dartNum, min, max);
	return dartNum - initial;
}