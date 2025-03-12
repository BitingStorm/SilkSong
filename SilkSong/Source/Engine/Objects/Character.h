/**
 * @file	Character.h
 * @brief   角色类定义，玩家控制器功能的延申，适合创建移动性较强的玩家操控的角色
 * @author	Arty
 **/



#pragma once
#include "Objects/Controller.h"


 /* 角色状态 */
enum class ECharacterMovementState : uint8
{
	Standing,
	Running,
	Flying
};

class Collider;

 /*----------------------------------
			    角色类
   ----------------------------------*/
class Character : public Controller
{
	DEFINE_SUPER(Controller)

public:
	Character();

	virtual void BeginPlay()override;

	virtual void Update(float deltaTime)override;

	
	/**
	 * @brief 输入水平移动命令
	 * @param[in] inputValue			  移动值，向右为正
	 * @param[in] bControlScale           角色方向是否由输入值控制，默认为是
	 **/
	void AddInputX(float inputValue,bool bControlScale = true);

	//获取角色状态
	ECharacterMovementState GetMovementState()const { return movementState; }

	//设置最大移动速度
	void SetMaxWalkingSpeed(float maxSpeed) { maxWalkingSpeed = (maxSpeed > 0 ? maxSpeed : 0); }

protected:
	void OnTouching(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult);

private:
	class SpriteRenderer* render;
	class BoxCollider* box;
	class RigidBody* rigid;
	
	ECharacterMovementState movementState;
	float maxWalkingSpeed;//最大移动速度
	bool bAddMoving;//是否正在输入移动命令
	int32 MoveFlag;//输入标记
};