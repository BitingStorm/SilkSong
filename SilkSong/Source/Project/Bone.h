#pragma once
#include "Bg.h"


class Bone :public Bg
{
public:
	Bone();
	virtual void Update(float deltaTime)override;

	void Init_(FVector2D size, FVector2D delta = {});
	void OverlapBegin(class Collider* hitComp, Collider* otherComp, Actor* otherActor);
	void OverlapEnd(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

protected:
	class BoxCollider* box;

	FVector2D aim;
};