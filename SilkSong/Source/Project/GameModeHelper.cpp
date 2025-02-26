#include "GameModeHelper.h"
#include "GameplayStatics.h"

GameMode* GameModeHelper::gameMode = nullptr;

GameMode* GameModeHelper::GetInstance()
{
	if (!gameMode)
	{
		gameMode = GameplayStatics::FindObjectOfClass<GameMode>();
	}
	return gameMode;
}

void GameModeHelper::ApplyDamage(IDamagable* damageCauser, IDamagable* damagedActor, float baseValue, EDamageType damageType)
{
	if (GameMode* pGameMode = GetInstance())
	{
		pGameMode->GetDamageSystem()->ApplyDamage(damageCauser, damagedActor, baseValue, damageType);
	}
}

void GameModeHelper::PlayBGMusic(std::string path)
{
	if (GameMode* pGameMode = GetInstance())
	{
		pGameMode->GetAudioPlayer(0)->Play(path, true);
	}
}

void GameModeHelper::PlayFXSound(std::string path)
{
	if (GameMode* pGameMode = GetInstance())
	{
		pGameMode->GetAudioPlayer(1)->Play(path);
	}
}

void GameModeHelper::PlayBGMusic_(std::string path)
{
	if (GameMode* pGameMode = GetInstance())
	{
		pGameMode->GetAudioPlayer(2)->Play(path, true);
	}
}