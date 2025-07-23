/**
 * @file	Struct.h
 * @brief	常用结构体定义或声明
 * @author	Arty
 **/


#pragma once
#include<iostream>
#include<string>
#include"Math/Vector2D.h"
#include"Math/Box2D.h"
#include"Math/Transform.h"


using FVector2D = Math::TVector2<float>;
using FIntVector2 = Math::TVector2<int32>;
using FVector2f = Math::TVector2<float>;
using FVector2d = Math::TVector2<double>;
using FBox2D = Math::TBox2<float>;
using FRect = Math::TBox2<float>;
using FIntBox2 = Math::TBox2<int32>;
using FTransform = Math::TTransform<float>;



/* 材质结合方式 */
enum class ECombinePattern :uint8 
{ 
	Smaller, 
	Mean, 
	Bigger 
};

/* 物理材质类 */
struct FPhysicsMaterial
{
	float friction;
	float bounciness;
	FPhysicsMaterial() :friction(0.4f), bounciness(0) {}
	FPhysicsMaterial(float friction, float bounciness)
		:friction(friction), bounciness(bounciness) {}

	FPhysicsMaterial operator+(const FPhysicsMaterial& another)
	{
		return FPhysicsMaterial(friction + another.friction, bounciness + another.bounciness);
	}

	FPhysicsMaterial operator*(float multi)
	{
		return FPhysicsMaterial(friction * multi, bounciness * multi);
	}

	//物理材质结合效果
	static FPhysicsMaterial Combine(
		const FPhysicsMaterial& m1, 
		const FPhysicsMaterial& m2, 
		ECombinePattern pattern = ECombinePattern::Mean
	);
};



class Actor;
class ActorComponent;

/* 碰撞结果 */
struct FHitResult
{
	FVector2D ImpactPoint;
	FVector2D ImpactNormal;
	Actor* HitObject;
	ActorComponent* HitComponent;

	FHitResult() :ImpactPoint(0, 0), ImpactNormal(0, 0), HitObject(nullptr), HitComponent(nullptr) {}
	FHitResult(const FVector2D& impactPoint, const FVector2D& impactNormal, Actor* hitObject, ActorComponent* hitComponent)
		:ImpactPoint(impactPoint), ImpactNormal(impactNormal), HitObject(hitObject), HitComponent(hitComponent) {}
};


/* 绑定方式 */
enum class EAttachmentRule :uint8 
{
	KeepRelative,
	KeepWorld
};

/* 绑定变换规则 */
struct FAttachmentTransformRules
{
	EAttachmentRule LocationRule;
	EAttachmentRule RotationRule;
	EAttachmentRule ScaleRule;
	
	FAttachmentTransformRules()
	{
		LocationRule = EAttachmentRule::KeepRelative;
		RotationRule = EAttachmentRule::KeepRelative;
		ScaleRule = EAttachmentRule::KeepRelative;
	}

	FAttachmentTransformRules(EAttachmentRule Location, EAttachmentRule Rotation, EAttachmentRule Scale)
		: LocationRule(Location), RotationRule(Rotation), ScaleRule(Scale) {}

	//保持相对变换，父对象变换会作用于子对象变换
	static FAttachmentTransformRules KeepRelativeTransform;
	
	//保持世界绝对变换，子对象依旧保持独立的变换
	static FAttachmentTransformRules KeepWorldTransform;
	
	//仅对位置保持相对变换
	static FAttachmentTransformRules KeepRelativeOnlyForLocation;
};