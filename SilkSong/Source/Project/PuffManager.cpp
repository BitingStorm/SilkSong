#include "PuffManager.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"


PuffManager::PuffManager()
{
	SpawnTimerHandle.Bind(0.015f, [this]() {
		if (spawnTimer == 20)
		{
			GameModeHelper::PlayFXSound("sound_boss_gushing");
		}
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
		effect->Init("effect_puff", -0.01f, FMath::RandReal(300 + spawnTimer * 3, 400 + spawnTimer * 4) * FVector2D::DegreeToVector(FMath::RandReal(0, 360)));
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.5f));
		spawnTimer++;
		if (spawnTimer > 235)
		{
			SpawnTimerHandle.Stop();
		}
		}, true, 1.5f);
	DestroyTimerHandle.Bind(7.f, [this]() 
		{
			for (int i = 0; i < 30; i++)
			{
				FVector2D unit = FVector2D::DegreeToVector(FMath::RandReal(0, 360));
				Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + unit * FMath::RandReal(0, 100));
				effect->Init("effect_puff", 0, FMath::RandReal(200, 600) * unit);
				effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.5f));
			}
			GameplayStatics::CreateObject<Effect>(GetWorldPosition())->Init("effect_splat");
		Destroy(); 
		GameModeHelper::PlayFXSound("sound_explode");
		}, false);
}
