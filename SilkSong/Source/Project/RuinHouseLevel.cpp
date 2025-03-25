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
	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->GetAudioPlayer(0)->Resume("tearcity_i");
	gameMode->GetAudioPlayer(2)->Resume("tearcity_o");
	GameModeHelper::PlayBGMusic_("tearcity_o");
	gameMode->RefreshVolume();
	gameMode->SetVolume(2, 0);
	GameModeHelper::PlayBGMusic("tearcity_i");
	GameplayStatics::CreateObject<LevelTransformer>({ 625,900 })->Init("TearCity", { -900,875 }, FRect({ -375.f,-1000.f }, { 1225.f,725.f }));
	GameplayStatics::CreateObject<LevelTransformer>({ -625,900 })->Init("GrimmTent", { 1400,875 }, FRect({ -1000.f,0.f }, { 1000.f,700.f }));
	Chair* chair = GameplayStatics::CreateObject<Chair>({ 0,950 });


	GameplayStatics::CreateObject<Bg>({ 0,700 })->Init("ruinhouse_0", -5);
	GameplayStatics::CreateObject<Bg>({ 0,700 })->Init("ruinhouse_1", 5, false, false);

	GameplayStatics::CreateObject<PlatForm>({ 0,1100 })->Init("", { 1200,200 }, {});

	GameplayStatics::CreateObject<PlatForm>({ -650,500 })->Init("", { 100,1000 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 650,500 })->Init("", { 100,1000 }, {});

	GameplayStatics::CreateObject<PlatForm>({ 0,250 })->Init("", { 1200,200 }, {});
}