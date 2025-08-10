#pragma once
#include "Objects/Character.h"
#include "Damagable.h"
#include "PropertyCarrier.h"


enum class ECharacterDirection : uint8
{
	LookForward,
	LookUp,
	LookDown,
};

enum class EPlayerState : uint8
{
	Normal,
	Defending,
	Scared,
};

enum class EPlayerAction : uint8
{
	Idle,
	Walk,
	Run,
	Jump,
	Fall,
	Land,
	Attack,
	Evade,
	Dash,
	Float,
	Sit,
	WallClimb,
	Defend,
	Scare,
	Die,
	Recover,
};

class Player :public Character, public IDamagable, public IPropertyCarrier
{
	DEFINE_SUPER(Character)

public:
	Player();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	virtual void EndPlay()override;

	virtual FDamageCauseInfo TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)override;

	virtual void ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)override;

	virtual void ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)override;

	virtual PropertyComponent* GetProperty()override;

	FVector2D GetCameraPos();

	int32 GetHealth()const;

	void AddHealth(int32 delta);

	float GetSilk()const;

	void AddSilk(int32 delta);

	int32 GetGeo()const;

	void AddGeo(int32 delta);

	int32 GetDart()const;

	void AddDart(int32 delta);

	void SetFloating(bool enable);

	void FinishDownAttack();

	void Bounce();

	void DieStart();

	void DieEnd();

	void Recover();

	void SitDown();

	void StandUp();

	void LeaveUp();

	void LeaveWall();

	void Defend(bool enable);

	void Scare(bool enable, std::string anim = "");

	bool IsSwimming() const;

protected:
	virtual void SetupInputComponent(InputComponent* inputComponent)override;

	void OnEnter(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);
	void OnStay(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);

private:
	class SpriteRenderer* render;
	class SpriteRenderer* render_light;
	class BoxCollider* box;
	class BoxCollider* hurtBox;
	class PlayerAnimator* ani;
	class RigidBody* rigid;
	class Camera* camera;
	class ParticleSystem* particle;
	class DamageResponseComponent* damageResponse;
	class PlayerPropertyComponent* playerProperty;
	class GameUI* ui;
	class EscUI* ui_esc;

	Timer BlinkTimer;
	Timer DieTimer;
	Timer RecoverTimer;
	Timer LowHealthTimer;
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
	float climbY;

	void SpawnWetLandEffect() const;
};