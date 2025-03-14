#include "SoulOrbManager.h"
#include "SoulOrb.h"
#include "GameplayStatics.h"


SoulOrbManager::SoulOrbManager()
{
	FVector2D multi[4] = { {1,0},{-1,0},{0,1},{0,-1} };

	for (int i = 0; i < 4; i++)
	{
		orbs[i] = GameplayStatics::CreateObject<SoulOrb>(multi[i] * 300);
		orbs[i]->AttachTo(this);
	}

	DestroyTimerHandle.Bind(4.f, [this]() {Destroy(); });
}

void SoulOrbManager::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	AddRotation(deltaTime * 75);

	reverseTimer += deltaTime;
	if (reverseTimer > 2.f)
	{
		if (GetOwner())
		{
			SetLocalPosition(GetWorldPosition());
			SetLocalScale(FVector2D(-GetWorldScale().x, 1));
			DetachFrom(GetOwner());
		}
		AddPosition(FVector2D(GetLocalScale().x * deltaTime * 750, 0));
	}
}
