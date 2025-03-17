#pragma once
#include "Objects/Actor.h"



class Gate :public Actor
{
public:
	Gate();

	virtual void Update(float deltaTime)override;

	void Open();

	void Close();

protected:
	class BoxCollider* box;
	class SpriteRenderer* render;

	bool bIsOpen = true;
}; 
