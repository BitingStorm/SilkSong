#include "RuinHouseLevel.h"
#include "PlatForm.h"
#include "Bg.h"
#include "Player.h"
#include "GameModeHelper.h"
#include "LevelTransformer.h"
#include "Chair.h"

RuinHouseLevel::RuinHouseLevel()
{
	SetDefaultController<Player>();
	GameModeHelper::PlayBGMusic_("tearcity_o");
	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->RefreshVolume();
	gameMode->SetVolume(2, 0);
	GameModeHelper::PlayBGMusic("tearcity_i");
	GameplayStatics::CreateObject<LevelTransformer>({ 625,900 })->Init("TearCity", {-900,875}, FRect({ -375.f,-1000.f }, { 1125.f,725.f }));
	Chair* chair = GameplayStatics::CreateObject<Chair>({ 0,950 });


	Bg* bg0 = GameplayStatics::CreateObject<Bg>({ 0,700 }); bg0->Init("ruinhouse_0", -5);
	Bg* bg1 = GameplayStatics::CreateObject<Bg>({ 0,700 }); bg1->Init("ruinhouse_1", 5, false, false);

	GameplayStatics::CreateObject<PlatForm>({ 0,1100 })->Init("", { 1200,200 }, {});

	GameplayStatics::CreateObject<PlatForm>({ -650,500 })->Init("", { 100,1000 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 650,500 })->Init("", { 100,1000 }, {});

	GameplayStatics::CreateObject<PlatForm>({ 0,250 })->Init("", { 1200,200 }, {});

}
