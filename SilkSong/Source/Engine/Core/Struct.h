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
enum class ECombinePattern :unsigned char { Min, Mid, Max };

/* 物理材质类 */
struct FPhysicsMaterial
{
	float friction;
	float bounciness;
	FPhysicsMaterial() :friction(0.4f), bounciness(0) {}
	FPhysicsMaterial(float friction, float bounciness)
		:friction(friction), bounciness(bounciness) {}

	//物理材质结合效果
	static FORCEINLINE FPhysicsMaterial Combine(const FPhysicsMaterial& m1, const FPhysicsMaterial& m2, ECombinePattern pattern = ECombinePattern::Mid)
	{
		if (pattern == ECombinePattern::Mid)
		{
			return FPhysicsMaterial((m1.friction + m2.friction) * 0.5f, (m1.bounciness + m2.bounciness) * 0.5f);
		}
		else
		{
			return pattern == ECombinePattern::Min ? FPhysicsMaterial(FMath::Min(m1.friction, m2.friction), FMath::Min(m1.bounciness, m2.bounciness))
				: FPhysicsMaterial(FMath::Max(m1.friction, m2.friction), FMath::Max(m1.bounciness, m2.bounciness));
		}
	}
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