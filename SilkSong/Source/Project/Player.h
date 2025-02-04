#pragma once
#include "Objects/Character.h"
#include "Tools/Timer.h"


enum class ECharacterDirection : uint8
{
	LookForward,
	LookUp,
	LookDown,
};

DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(OnHurtEvent)
DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(OnCureEvent)

class Player :public Character
{
	DEFINE_SUPER(Character)

public:
	Player();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	void TakeDamage(FVector2D normal);

	FVector2D GetCameraPos();

	int32 GetHealth()const { return health; }

	void AddHealth(int32 delta);

	float GetSilk()const { return silk; }

	void AddSilk(int delta);

	int32 GetGeo()const { return geo; }

	void AddGeo(int32 delta) { geo += delta; }

	int32 GetDart()const { return dartNum; }

	void AddDart(int32 delta);

	void SetFloating(bool enable);

	void Bounce();

	void Grab();

	void DieStart();

	void DieEnd();

	void Recover();

	void SitDown();

	void StandUp();

	void LeaveUp();

	void LeaveWall();

	OnCureEvent cureEvent;
	OnHurtEvent hurtEvent;

protected:
	virtual void SetupInputComponent(InputComponent* inputComponent)override;

	void OnEnter(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const HitResult& hitResult);
	void OnStay(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const HitResult& hitResult);

private:
	class SpriteRenderer* render;
	class SpriteRenderer* render_light;
	class BoxCollider* box;
	class BoxCollider* hurtBox;
	class PlayerAnimator* ani;
	class RigidBody* rigid;
	class Camera* camera;
	class AudioPlayer* audio;
	class ParticleSystem* particle;
	class GameUI* ui;

	Timer BlinkTimer;
	Timer DieTimer;
	Timer RecoverTimer;
	int blinkTimes;

	bool bGround;
	bool bEvading;
	bool bDashing;
	bool bFloating;
	bool bRushing;
	bool bRushFlag;
	bool bSitting;
	bool bWall;

	float lastJumpTime;
	float lastAttackTime;
	float lastEvadeTime;
	float lastDashTime;
	float lastFloatTime;
	float lastThrowTime;
	int32 attackFlag;
	float lookFlag;
	int32 walkLock;
	int32 jumpFlag;
	ECharacterDirection direction;

	int32 health;
	int32 silk;
	int32 geo;
	int32 dartNum;

	void SpawnDashEffect();

	void SpawnWetLandEffect();

	void SpawnWetWalkEffect();
};