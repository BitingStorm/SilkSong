#pragma once
#include "Enemy.h"


class SoulMaster :public Enemy
{
	DEFINE_SUPER(Enemy)

public:
	SoulMaster();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);
	
	virtual void SpawnGeos()override;

	virtual void Die()override;

private:
	Animation roar;
	Animation idle;
	Animation turn;
	Animation startteleport;
	Animation endteleport;
	Animation startsummon;
	Animation summon;
	Animation endsummon;
	Animation startquake;
	Animation quake;
	Animation startdash;
	Animation dash;
	Animation startstun;
	Animation stuntransition;
	Animation stuntransition_;
	Animation stun;
	Animation die;

	AnimEdge turn_to_idle;
	AnimEdge startteleport_to_endteleport;
	AnimEdge endteleport_to_idle;
	AnimEdge startsummon_to_summon;
	AnimEdge summon_to_endsummon;
	AnimEdge endsummon_to_idle;
	AnimEdge startquake_to_quake;
	AnimEdge startdash_to_dash;
	AnimEdge startstun_to_stuntransition;
	AnimEdge stuntransition_to_stun;
	AnimEdge stun_to_stuntransition_;
	AnimEdge stuntransition__to_idle;

	AnimationDelegate requake;
	AnimationDelegate cameraShake;
	AnimationDelegate stunPause;
	bool quakeLock = false;

	Timer RoarTimerHandle;
	int32 roarTimer;
	Timer BehaviorTimerHandle;
	FVector2D moveSpeed = FVector2D::ZeroVector;
	Timer RecoverTimerHandle;
	int32 stunTimer;
	Timer Behavior_1TimerHandle;
	int32 deathShakeTimer;
	Timer SummonTimerHandle;
	Timer DieTimerHandle;

	int32 behaviorFlag = 0;
	
	int32 state = 0;
	int32 behaveTimer = 0;

	void Move();

	void Behave();

	void Behave_1();

	FVector2D ClampPosX(FVector2D pos);
};
