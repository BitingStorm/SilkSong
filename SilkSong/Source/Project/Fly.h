#pragma once
#include "Enemy.h"


class Fly :public Enemy
{
	DEFINE_SUPER(Enemy)

public:
	Fly();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	virtual void SpawnGeos()override;

	virtual void Die()override;

private:
	Animation idle;
	Animation turn;
	Animation startchase;
	Animation chase;
	Animation die;
	AnimEdge idle_to_startchase;
	AnimEdge turn_to_idle;
	AnimEdge turn_to_chase;
	AnimEdge startchase_to_chase;
	AnimEdge chase_to_idle;

	bool bChasing = false;
};
