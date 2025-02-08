#include "RigidBody.h"
#include "Collider.h"
#include "Objects/Actor.h"





RigidBody::~RigidBody()
{
	for (auto& collider : colliders)collider->rigidAttached = nullptr;
}

void RigidBody::Update(float deltaTime)
{
	this->deltaTime = deltaTime;
	if (!pOwner || !bIsEnabled)return;

	if (bMoveable)
	{
		if (bGravityEnabled)
		{
			velocity.y += gravity * deltaTime;
		}
		for (auto& collider : colliders)
		{
			if (collider->mode != CollisionMode::Collision)continue;
			for (auto& another : collider->collisions)
			{
				if (another->mode != CollisionMode::Collision)continue;
				RestrictVelocity(-collider->CollisionHit(another).ImpactNormal, FPhysicsMaterial::Combine(collider->material, another->material), another->rigidAttached);
			}
		}
		for (auto& collider : colliders)
		{
			if (collider->mode != CollisionMode::Collision)continue;
			for (auto& another : collider->collisions)
			{
				if (another->mode != CollisionMode::Collision || another->rigidAttached)continue;
				RestrictVelocity(-collider->CollisionHit(another).ImpactNormal, FPhysicsMaterial::Combine(collider->material, another->material));
			}
		}

		FVector2D offset = velocity.ClampAxes(-maxSpeed, maxSpeed) * deltaTime;
		pOwner->AddPosition(FVector2D(FMath::IsSmallNumber(offset.x) ? 0 : offset.x, FMath::IsSmallNumber(offset.y) ? 0 : offset.y));

		if (linearDrag)
		{
			if (!FMath::IsSmallNumber(velocity.x))
			{
				float buffer = velocity.x - velocity.x * linearDrag * deltaTime / mass;
				velocity.x = (velocity.x < 0) != (buffer < 0) ? 0 : buffer;
			}
			if (!FMath::IsSmallNumber(velocity.y))
			{
				float buffer = velocity.y - velocity.y * linearDrag * deltaTime / mass;
				velocity.y = (velocity.y < 0) != (buffer < 0) ? 0 : buffer;
			}
		}
	}

	if (bRotatable)
	{
		float offset = angularVelocity * deltaTime;
		pOwner->AddRotation(FMath::IsSmallNumber(offset) ? 0 : offset);

		if (angularDrag && !FMath::IsSmallNumber(angularVelocity))
		{
			float buffer = angularVelocity - angularVelocity * angularDrag * deltaTime / mass;
			angularVelocity = (angularVelocity < 0) != (buffer < 0) ? 0 : buffer;
		}
	}
}

void RigidBody::RestrictVelocity(FVector2D impactNormal, const FPhysicsMaterial& material, RigidBody* another)
{
	FVector2D tangentVector = { impactNormal.y, -impactNormal.x };

	FVector2D normalVelocity = FVector2D::ProjectVector(velocity, impactNormal);
	FVector2D tangentVelocity = FVector2D::ProjectVector(velocity, tangentVector);

	float friction = material.friction;
	float bounciness = material.bounciness;

	/**
	 * ��һ���崦���߼�
	 **/
	if (!another)
	{
		if (FVector2D::DotProduct(velocity, impactNormal) < 0)
		{
			float multiplier = 1.f - normalVelocity.Size() * friction * FMath::InvSqrt(tangentVelocity.SizeSquared());
			multiplier = FMath::Clamp(multiplier, 0.0f, 1.0f);
			velocity = tangentVelocity * multiplier - bounciness * normalVelocity;
		}
		return;
	}


	/**
	 * ˫���嵯����ײ�����߼�������Ħ����
	 **/
	FVector2D anotherNormalVelocity = FVector2D::ProjectVector(another->velocity, impactNormal);
	FVector2D anotherTangentVelocity = FVector2D::ProjectVector(another->velocity, tangentVector);

	if (FVector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactNormal) >= 0)return;//ȷ��������������

	FVector2D normalVelocity_ = normalVelocity;
	normalVelocity = ((mass - bounciness * another->mass) * normalVelocity + (1 + bounciness) * another->mass * anotherNormalVelocity) / (mass + another->mass);
	anotherNormalVelocity = ((another->mass - bounciness * mass) * anotherNormalVelocity + (1 + bounciness) * mass * normalVelocity_) / (mass + another->mass);

	velocity = normalVelocity + tangentVelocity;
	another->velocity = anotherNormalVelocity + anotherTangentVelocity;
}