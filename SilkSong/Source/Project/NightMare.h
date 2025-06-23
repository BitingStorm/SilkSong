#pragma once
#include "Enemy.h"


class NightMare :public Enemy
{
	DEFINE_SUPER(Enemy)

public:
	NightMare();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

protected:
	void OnHit(class Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);

	virtual void SpawnGeos()override;

	virtual void Die()override;

private:
	Animation fingerout;
	Animation fingerclick;
	Animation fingerin;
	Animation stand;
	Animation idle;
	Animation bow;
	Animation startteleport;
	Animation endteleport;
	Animation startspike;
	Animation spike;
	Animation startballoon;
	Animation balloon;
	Animation cast;
	Animation startairdash;
	Animation airdash;
	Animation startdash;
	Animation dash;
	Animation enddash;
	Animation startslash;
	Animation slash;
	Animation startuppercut;
	Animation uppercut;
	Animation stun;
	Animation fly;
	Animation turn;
	Animation die;
	Animation scream;

	AnimEdge fingerout_to_fingerclick;
	AnimEdge fingerclick_to_fingerin;
	AnimEdge startteleport_to_endteleport;
	AnimEdge endteleport_to_stand;
	AnimEdge startspike_to_spike;
	AnimEdge startballoon_to_balloon;
	AnimEdge startairdash_to_airdash;
	AnimEdge startdash_to_dash;
	AnimEdge dash_to_enddash;
	AnimEdge startslash_to_slash;
	AnimEdge slash_to_startuppercut;
	AnimEdge startuppercut_to_uppercut;
	AnimEdge stun_to_fly;
	AnimEdge turn_to_fly;

	AnimationDelegate spawnBall;
	AnimationDelegate dieShake;
	AnimationDelegate stunPause;
	AnimationDelegate fingerEnd;

	class BoxCollider* box;
	Animator* ani_;
	class SpriteRenderer* render_;
	class Bg* beam;

	Timer RoarTimerHandle;
	int32 roarTimer;

	Timer ClickTimerHandle;
	Timer BowTimerHandle;
	Timer BehaviorTimerHandle;
	Timer TeleportTimerHandle;
	Timer CastTimerHandle;
	Timer BalloonTimerHandle;
	Timer RecoverTimerHandle;
	Timer DieTimerHandle;
	int32 balloonSpawnFlag = 0;
	int32 castTimer = 0;
	int32 behaviorFlag = 0;

	int32 stunTimer;
	int32 balloonTimer;

	void Move();

	void Behave();

	void SpawnBall();

	FVector2D ClampPosX(FVector2D pos);
};
