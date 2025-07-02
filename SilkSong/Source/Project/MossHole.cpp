#include "MossHole.h"
#include "PlatForm.h"
#include "Bg.h"
#include "Player.h"
#include "GameModeHelper.h"


MossHoleLevel::MossHoleLevel()
{
	SetDefaultController<Player>();

	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->RefreshVolume();
	GameModeHelper::PlayBGMusic("mossgrotto");
	

	GameplayStatics::CreateObject<Bg>({ 0,700 })->Init("mosshole_0", -10);
	GameplayStatics::CreateObject<PlatForm>({ 0,1100 })->Init("", { 1200,200 }, {});
}
