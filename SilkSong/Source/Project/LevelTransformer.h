#pragma once
#include "Objects/Actor.h"



class LevelTransformer :public Actor
{
public:
	LevelTransformer();

	void Init(std::string level, FVector2D pos, FRect rec);

protected:
	class BoxCollider* box;

	void OnBeginOverlap(class Collider* hitComp, Collider* otherComp, Actor* otherActor);

	std::string aimLevel{};

	FVector2D aimPos{};

	FRect aimRect{};
};