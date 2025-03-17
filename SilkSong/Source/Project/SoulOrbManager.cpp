#include "SoulOrbManager.h"
#include "SoulOrb.h"
#include "GameplayStatics.h"


SoulOrbManager::SoulOrbManager()
{
	FVector2D multi[4] = { {1,0},{-1,0},{0,1},{0,-1} };

	for (int i = 0; i < 4; i++)
	{
		orbs[i] = GameplayStatics::CreateObject<SoulOrb>(multi[i] * 60);
		orbs[i]->AttachTo(this);
		orbs[i]->SetLocalScale(FVector2D(0.2f,0.2f));
	}

	DestroyTimerHandle.Bind(4.f, [this]() {Destroy(); });
}

void SoulOrbManager::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	AddRotation(deltaTime * 75);

	reverseTimer += deltaTime;

	if (reverseTimer < 0.5f)
	{
		SetLocalScale(FVector2D::UnitVector * (1 + reverseTimer * 8));
	}


	if (reverseTimer > 2.f)
	{
		if (GetOwner())
		{
			SetLocalPosition(GetWorldPosition());
			SetLocalScale(FVector2D(-GetWorldScale().x, GetWorldScale().y));
			DetachFrom(GetOwner());
		}
		AddPosition(FVector2D((GetLocalScale().x > 0 ? 1.f : -1.f) * deltaTime * 750, 0));
	}
}
