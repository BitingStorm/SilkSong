/**
 * @file	Struct.h
 * @brief	���ýṹ�嶨�������
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



/* ���ʽ�Ϸ�ʽ */
enum class ECombinePattern :uint8 
{ 
	Smaller, 
	Mean, 
	Bigger 
};

/* ��������� */
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

	//������ʽ��Ч��
	static FPhysicsMaterial Combine(
		const FPhysicsMaterial& m1, 
		const FPhysicsMaterial& m2, 
		ECombinePattern pattern = ECombinePattern::Mean
	);
};



class Actor;
class ActorComponent;

/* ��ײ��� */
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


/* �󶨷�ʽ */
enum class EAttachmentRule :uint8 
{
	KeepRelative,
	KeepWorld
};

/* �󶨱任���� */
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

	//������Ա任��������任���������Ӷ���任
	static FAttachmentTransformRules KeepRelativeTransform;
	
	//����������Ա任���Ӷ������ɱ��ֶ����ı任
	static FAttachmentTransformRules KeepWorldTransform;
	
	//����λ�ñ�����Ա任
	static FAttachmentTransformRules KeepRelativeOnlyForLocation;
};