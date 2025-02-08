/**
 * @file	Struct.h
 * @brief	常用结构体定义或声明
 * @author	Arty
 **/


#pragma once
#include<iostream>
#include<string>
#include"Vector2D.h"
#include"Box2D.h"


namespace Math
{
	template<typename T> struct TVector2;
	template<typename T> struct TBox2;
}


using FVector2D = Math::TVector2<float>;
using FIntVector2 = Math::TVector2<int32>;
using FVector2f = Math::TVector2<float>;
using FVector2d = Math::TVector2<double>;
using FBox2D = Math::TBox2<float>;
using FRect = Math::TBox2<float>;
using FIntBox2 = Math::TBox2<int32>;


/* 场景属性类 */
struct FTransform
{
	FVector2D position;
	float rotation;
	FVector2D scale;

	FTransform() :position(0, 0), rotation(0), scale(1, 1) {}
	FTransform(const FVector2D& pos, float angle, const FVector2D& scale)
		:position(pos), rotation(angle), scale(scale) {}

	FORCEINLINE FTransform Indentity() const
	{
		return FTransform(FVector2D::ZeroVector, 0, FVector2D::UnitVector);
	}
};


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