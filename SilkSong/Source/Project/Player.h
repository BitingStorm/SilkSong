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


class Player :public Character, public IDamagable, public IPropertyCarrier
{
	DEFINE_SUPER(Character)

public:
	Player();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

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

	void Bounce();

	void Grab();

	void DieStart();

	void DieEnd();

	void Recover();

	void SitDown();

	void StandUp();

	void LeaveUp();

	void LeaveWall();

	void Defend(bool enable);

	void Scare(bool enable, std::string anim = "");

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

	void SpawnWetLandEffect() const;
};