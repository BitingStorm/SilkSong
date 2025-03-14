#include "BossLevel.h"
#include "GameModeHelper.h"
#include "LevelTransformer.h"
#include "SoulMaster.h"
#include "PlatForm.h"
#include "Bg.h"
#include "RainProducer.h"
#include "Player.h"


BossLevel::BossLevel()
{
	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->RefreshVolume();
	gameMode->GetAudioPlayer(0)->Stop("tearcity_i");
	gameMode->GetAudioPlayer(2)->Stop("tearcity_o");
	GameModeHelper::PlayBGMusic("tearcity_boss");

	GameplayStatics::CreateObject<SoulMaster>({ -200,600 });

	Bg* bg0 = GameplayStatics::CreateObject<Bg>(); bg0->Init("tearcity_0", -30);
	Bg* bg1 = GameplayStatics::CreateObject<Bg>({ 0,1000 }); bg1->Init("tearcity_4", -10);
	Bg* bg2 = GameplayStatics::CreateObject<Bg>({ -1200,600 }); bg2->Init("tearcity_2", -15);
	Bg* bg3 = GameplayStatics::CreateObject<Bg>({ 1000,1300 }); bg3->Init("tearcity_5", -20);

	PlatForm* plat = GameplayStatics::CreateObject<PlatForm>({ 0,1075 }); plat->Init("tearcity_glassfloor", { 1500,154 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -925,1030 })->Init("tearcity_additions", { 346,64 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 925,1030 })->Init("tearcity_additions", { 346,64 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1150,500 })->Init("", { 100,1400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1150,500 })->Init("", { 100,1400 }, {});

	GameplayStatics::CreateObject<RainProducer>({ -500,-500 });

	if (Player* player = GameplayStatics::FindObjectOfClass<Player>())
	{
		player->Scare(true);
		player->EnableInput(false);
	}
}
