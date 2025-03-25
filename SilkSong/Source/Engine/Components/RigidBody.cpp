#include "RigidBody.h"
#include "Collider.h"
#include "Objects/Actor.h"
#include "Core/World.h"




RigidBody::RigidBody()
{
	mainWorld.GameRigids.insert(this);
}

RigidBody::~RigidBody()
{
	mainWorld.GameRigids.erase(this);
	for (auto& collider : colliders)collider->rigidAttached = nullptr;
}

void RigidBody::Update(float deltaTime)
{
	if (!pOwner || !bIsEnabled)return;

	if (bMoveable && linearDrag)
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

void RigidBody::PreciseUpdate(float deltaTime)
{
	if (!bMoveable)
	{
		return;
	}
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
			FHitResult hitResult = collider->CollisionHit(another);
			RestrictVelocity(-hitResult.ImpactNormal, FPhysicsMaterial::Combine(collider->material, another->material), another->rigidAttached, true);
		}
	}

	FVector2D offset = velocity.ClampAxes(-maxSpeed, maxSpeed) * deltaTime;
	pOwner->AddPosition(FVector2D(FMath::IsSmallNumber(offset.x) ? 0 : offset.x, FMath::IsSmallNumber(offset.y) ? 0 : offset.y));
}

void RigidBody::RegisterDontDestroy()
{
	mainWorld.OverallRigids.insert(this);
}

void RigidBody::RestrictVelocity(FVector2D impactNormal, const FPhysicsMaterial& material, RigidBody* another, bool isStay)
{
	FVector2D tangentVector = { impactNormal.y, -impactNormal.x };

	FVector2D normalVelocity = FVector2D::ProjectVector(velocity, impactNormal);
	FVector2D tangentVelocity = FVector2D::ProjectVector(velocity, tangentVector);

	float friction = material.friction;
	float bounciness = FMath::Clamp(material.bounciness, 0.0f, 1.0f);

	/**
	 * 单一刚体处理逻辑
	 **/
	if (!another || !another->bMoveable)
	{
		if (FVector2D::DotProduct(velocity, impactNormal) < 0)
		{
			float multiplier = 1.f - normalVelocity.Size() * friction * FMath::InvSqrt(tangentVelocity.SizeSquared());
			multiplier = FMath::Clamp(multiplier, 0.0f, 1.0f);
			velocity = tangentVelocity * multiplier - bounciness * (isStay ? FVector2D::ZeroVector : normalVelocity);
		}
		return;
	}


	/**
	 * 双刚体弹性碰撞处理逻辑（忽略摩擦）
	 **/
	FVector2D anotherNormalVelocity = FVector2D::ProjectVector(another->velocity, impactNormal);
	FVector2D anotherTangentVelocity = FVector2D::ProjectVector(another->velocity, tangentVector);

	if (FVector2D::DotProduct(normalVelocity - anotherNormalVelocity, impactNormal) >= 0)return;//确保有相碰的趋势

	FVector2D normalVelocity_ = normalVelocity;
	normalVelocity = ((mass - bounciness * another->mass) * normalVelocity + (1 + bounciness) * another->mass * anotherNormalVelocity) / (mass + another->mass);
	anotherNormalVelocity = ((another->mass - bounciness * mass) * anotherNormalVelocity + (1 + bounciness) * mass * normalVelocity_) / (mass + another->mass);

	velocity = normalVelocity + tangentVelocity;
	another->velocity = anotherNormalVelocity + anotherTangentVelocity;
}