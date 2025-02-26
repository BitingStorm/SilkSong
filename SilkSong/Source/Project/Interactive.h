#pragma once
#include "Bg.h"
#include "Damagable.h"


class Interactive :public Bg, public IDamagable
{
public:
	Interactive();

	virtual void Update(float deltaTime)override;

	void Init_(std::string path, FVector2D boxSize, bool flag = false);

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override;

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override {}

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	class BoxCollider* box;
	class DamageResponseComponent* damageResponse;
	std::string path_;
	bool flag;
};
