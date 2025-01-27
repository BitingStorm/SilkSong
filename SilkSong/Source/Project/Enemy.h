#pragma once
#include "Objects/Actor.h"
#include"Components/Animator.h"


class Enemy :public Actor
{
	DEFINE_SUPER(Actor)

public:
	Enemy();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	void TakeDamage(Vector2D normal, bool gain = true);

	bool IsDead()const {return bIsDead;}

protected:
	void OnOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	virtual void Die();

	virtual void SpawnGeos() = 0;

	class SpriteRenderer* render;
	class SpriteRenderer* render_death;
	class CircleCollider* circle;
	class RigidBody* rigid;
	class AudioPlayer* audio;
	Animator* ani;

	class Player* player;

	int blood = 10;
	bool bIsDead = false;
};