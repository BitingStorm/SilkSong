/**
 * @file	RigidBody.h
 * @brief	刚体组件定义，简要模拟2D游戏场景中的物理运动规律
 * @author	Arty
 **/

#pragma once
#include"Components/ActorComponent.h"



/*----------------------------------
			  刚体组件
  ----------------------------------*/
class RigidBody final :public ActorComponent
{
	friend class Collider;

public:
	RigidBody();
	virtual ~RigidBody();
	virtual void Update(float deltaTime) override;
	void PreciseUpdate(float deltaTime);

	//设置重力
	void SetGravity(float gravity) { this->gravity = gravity; }
	
	//设置速度
	void SetVelocity(FVector2D velocity) { this->velocity = velocity; }
	
	//设置速度最大值
	void SetMaxSpeed(float maxSpeed) { this->maxSpeed = maxSpeed; }
	
	//设置角速度
	void SetAngularVelocity(float angularVelocity) { this->angularVelocity = angularVelocity; }
	
	//设置质量
	void SetMass(float mass) { this->mass = mass; }
	
	//添加冲量
	void AddImpulse(FVector2D pulse) { if (bMoveable) { velocity += pulse / mass; } }

	//是否可移动
	void SetMoveable(bool moveable) { this->bMoveable = moveable; if (!moveable)velocity = FVector2D::ZeroVector; }
	
	//是否启用重力
	void SetGravityEnabled(bool enabled) { this->bGravityEnabled = enabled; }
	
	//是否可旋转
	void SetRotatable(bool rotatable) { this->bRotatable = rotatable; if (!rotatable)angularVelocity = 0; }

	//设置线性阻尼
	void SetLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }

	//设置角阻尼
	void SetAngularDrag(float angularDrag) { this->angularDrag = angularDrag; }

	//获取速度
	FVector2D GetVelocity()const { return velocity; }

	virtual void RegisterDontDestroy()override;

private:
	FVector2D velocity{};
	float maxSpeed = 5000.0f;
	bool bMoveable = true;
	
	float gravity = 980.f;
	bool bGravityEnabled = true;

	float angularVelocity = 0;
	bool bRotatable = true;
	float mass = 1.0f;

	float linearDrag = 0.05f;
	float angularDrag = 0.f;

	std::unordered_set<Collider*> colliders;

	//运动模拟
	void RestrictVelocity(FVector2D impactNormal, const FPhysicsMaterial& material, RigidBody* another = nullptr, bool isStay = false);
};