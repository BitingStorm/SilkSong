#pragma once
#include "PropertyComponent.h"

class PlayerPropertyComponent :public PropertyComponent
{
	int32 silk;
	int32 geo;
	int32 dartNum;

public:
	PlayerPropertyComponent();

	int32 GetSilk()const { return silk; }

	int32 AddSilk(int32 delta, int32 max = 9, int32 min = 0);

	int32 GetGeo()const { return geo; }

	int32 AddGeo(int32 delta, int32 max = 9999, int32 min = 0);

	int32 GetDart()const { return dartNum; }

	int32 AddDart(int32 delta, int32 max = 15, int32 min = 0);
};