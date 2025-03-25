#include "TearCityLevel.h"
#include "Bug.h"
#include "Fly.h"
#include "PlatForm.h"
#include "Bg.h"
#include "Interactive.h"
#include "RainProducer.h"
#include "Animated.h"
#include "Chest.h"
#include "GameModeHelper.h"
#include "LevelTransformer.h"


TearCityLevel::TearCityLevel()
{
	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->GetAudioPlayer(0)->Resume("tearcity_i");
	gameMode->GetAudioPlayer(2)->Resume("tearcity_o");
	GameModeHelper::PlayBGMusic_("tearcity_o");
	gameMode->RefreshVolume();
	gameMode->SetVolume(2, gameMode->GetVolume(0));
	GameModeHelper::PlayBGMusic("tearcity_i");
	GameplayStatics::CreateObject<LevelTransformer>({ -1050, 875 })->Init("RuinHouse", { 550,900 }, FRect({ -100.f,250.f }, { 100.f,750.f }));
	GameplayStatics::CreateObject<LevelTransformer>({ 1850, 875 })->Init("BossHouse", { -1200,900 }, FRect({ -675.f,-250.f }, { 475.f,675.f }));

	Bg* bg0 = GameplayStatics::CreateObject<Bg>(); bg0->Init("tearcity_0", -30);
	Bg* bg1 = GameplayStatics::CreateObject<Bg>({ 2500,350 }); bg1->Init("tearcity_1", -20);
	Bg* bg2 = GameplayStatics::CreateObject<Bg>({ -1200,300 }); bg2->Init("tearcity_2", -15);
	Bg* bg3 = GameplayStatics::CreateObject<Bg>({ 500,1000 }); bg3->Init("tearcity_3", -10);


	Bg* ev0 = GameplayStatics::CreateObject<Bg>({ 0,600 }); ev0->Init("tearcity_environment_0", -3, true);

	Animated* waterfall = GameplayStatics::CreateObject<Animated>({ -60,160 }); waterfall->Init("water_fountain", 0.05f, 0); waterfall->AttachTo(ev0);
	waterfall = GameplayStatics::CreateObject<Animated>({ 50,160 }); waterfall->Init("water_fountain", 0.04f, -1); waterfall->AttachTo(ev0);
	waterfall = GameplayStatics::CreateObject<Animated>({ -200,160 }); waterfall->Init("water_fountain", 0.08f, -4); waterfall->AttachTo(ev0);
	waterfall = GameplayStatics::CreateObject<Animated>({ -150,160 }); waterfall->Init("water_fountain", 0.06f, -7); waterfall->AttachTo(ev0);
	waterfall = GameplayStatics::CreateObject<Animated>({ 150,160 }); waterfall->Init("water_fountain", 0.08f, -6); waterfall->AttachTo(ev0);

	Bg* ev1 = GameplayStatics::CreateObject<Bg>({ 0,975 }); ev1->Init("tearcity_environment_1", -2);
	for (int i = 0; i < 5; i++) {
		Animated* rainland = GameplayStatics::CreateObject<Animated>({ -225 + float(i) * FMath::RandInt(80,120),-50 });
		rainland->Init("rain_land", 0.08f, -2);
		rainland->AttachTo(ev1);
	}

	Bg* ev2 = GameplayStatics::CreateObject<Bg>({ -1000,800 }); ev2->Init("tearcity_environment_2", -3);
	Bg* ev3 = GameplayStatics::CreateObject<Bg>({ -700,450 }); ev3->Init("tearcity_environment_3", 2);

	Bg* ev4 = GameplayStatics::CreateObject<Bg>({ 500,1080 }); ev4->Init("tearcity_water", 4);

	for (int i = 0; i < 10; i++) {
		Animated* watertop = GameplayStatics::CreateObject<Animated>({ -1500 + float(i) * 300,-75 });
		watertop->Init("water_top", 0.08f, 4); watertop->AttachTo(ev4);
	}
	for (int i = 0; i < 10; i++) {
		Animated* rainland = GameplayStatics::CreateObject<Animated>({ -1500 + float(i) * FMath::RandInt(100,500),-75 });
		rainland->Init("rain_land", 0.08f, 4); rainland->AttachTo(ev4);
		rainland->SetLocalScale(FVector2D(1.25, 1.25) * FMath::RandReal(0.8f, 1.2f));
	}


	Interactive* ev5 = GameplayStatics::CreateObject<Interactive>({ -850,800 });
	ev5->Init("tearcity_environment_19", -2); ev5->Init_("tearcity_environment_4", { 30,250 }, true);
	Interactive* ev6 = GameplayStatics::CreateObject<Interactive>({ -400,850 });
	ev6->Init("tearcity_environment_16", -2); ev6->Init_("tearcity_environment_5", { 20,210 });
	Bg* ev7 = GameplayStatics::CreateObject<Bg>({ 1200,875 }); ev7->Init("tearcity_environment_6", 5, false, false);
	Bg* ev8 = GameplayStatics::CreateObject<Bg>({ 800,680 }); ev8->Init("tearcity_environment_7", 0);
	Bg* ev9 = GameplayStatics::CreateObject<Bg>({ 900,980 }); ev9->Init("tearcity_environment_8", 5);
	GameplayStatics::CreateObject<Bg>({ 750,1075 })->Init("tearcity_environment_9", -2);
	Interactive* ev10 = GameplayStatics::CreateObject<Interactive>({ 1100,850 });
	ev10->Init("tearcity_environment_10", -2); ev10->Init_("tearcity_environment_14", {20,200});
	GameplayStatics::CreateObject<Bg>({ 1580,975 })->Init("tearcity_environment_11", 4);
	GameplayStatics::CreateObject<Bg>({ 1500,830 })->Init("tearcity_environment_12", -1);
	GameplayStatics::CreateObject<Bg>({ -600,1050 })->Init("tearcity_environment_13", -2);
	Interactive* ev14 = GameplayStatics::CreateObject<Interactive>({ 450,845 });
	ev14->Init("tearcity_environment_10", 5, false, false); ev14->Init_("tearcity_environment_14", { 20,200 });
	GameplayStatics::CreateObject<Bg>({ -785,950 })->Init("tearcity_environment_15", 5);
	Interactive* ev16 = GameplayStatics::CreateObject<Interactive>({ 300,500 });
	ev16->Init("tearcity_environment_17", -1, false, false); ev16->Init_("tearcity_environment_18", { 20,200 }, true);

	GameplayStatics::CreateObject<PlatForm>({ 650,400 })->Init("tearcity_plat_0", { 130,60 }, { 0,-25 });
	GameplayStatics::CreateObject<PlatForm>({ 300,700 })->Init("tearcity_plat_2", { 80,70 }, { 0,-10 });
	GameplayStatics::CreateObject<PlatForm>({ 800,750 })->Init("tearcity_plat_1", { 80,70 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 50,200 })->Init("tearcity_plat_4", { 350,70 }, { 0,-30 });

	PlatForm* plat = GameplayStatics::CreateObject<PlatForm>({ 0,1075 }); plat->Init("tearcity_floor", { 1125,210 }, {});
	for (int i = 0; i < 5; i++) {
		Animated* rainland = GameplayStatics::CreateObject<Animated>({ -500 + float(i) * FMath::RandInt(150,250),-100 });
		rainland->Init("rain_land", 0.08f, 4);
		rainland->SetLocalScale(FVector2D(1, 1) * FMath::RandReal(0.75f, 1.25f));
		rainland->AttachTo(plat);
	}

	GameplayStatics::CreateObject<PlatForm>({ -1300,1075 })->Init("tearcity_floor", { 1125,210 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1500,1075 })->Init("tearcity_floor", { 925,210 }, {-100,0});

	GameplayStatics::CreateObject<PlatForm>({ -1000,250 })->Init("", { 100,1100 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1850,250 })->Init("", { 100,1100 }, {});

	GameplayStatics::CreateObject<PlatForm>({ -650,1175 })->Init("", { 200,100 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 775,1175 })->Init("", { 450,100 }, {});


	GameplayStatics::CreateObject<Chest>({ 0,68 });

	GameplayStatics::CreateObject<Bug>({ 1100,890 }, 0.f, { -1.f,1.f });
	GameplayStatics::CreateObject<Bug>({ 650,300 });
	GameplayStatics::CreateObject<Bug>({ -500,890 });

	GameplayStatics::CreateObject<Fly>({ 0,800 });
	GameplayStatics::CreateObject<Fly>({ 450,500 }, 0.f, { -1.f,1.f });

	GameplayStatics::CreateObject<RainProducer>({ 500,-500 });
}