#include "MossHole.h"
#include "PlatForm.h"
#include "Bg.h"
#include "MossFloor.h"
#include "Player.h"
#include "GameModeHelper.h"
#include "Components/Camera.h"
#include "Animated.h"
#include "Plant.h"
#include "LevelTransformer.h"


MossHoleLevel::MossHoleLevel()
{
	SetDefaultController<Player>();

	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->RefreshVolume();
	GameModeHelper::PlayBGMusic("stream");
	GameModeHelper::PlayBGMusic("cicadachirp");
	
	GameplayStatics::CreateObject<LevelTransformer>({ -1850,550 })->Init("MossHole_0", { 2400,875 }, FRect({ -2000.f,650.f }, { 1900.f,750.f }));
	GameplayStatics::CreateObject<Bg>({ 100,650 })->Init("mosshole_0", -20, -13);
	GameplayStatics::CreateObject<Bg>({ -1300,600 })->Init("mosshole_bg_0", -5);
	GameplayStatics::CreateObject<Bg>({ 300,875 })->Init("mosshole_bg_1", -2);
	GameplayStatics::CreateObject<Bg>({ -1700,550 })->Init("mosshole_cave_light", -2);
	GameplayStatics::CreateObject<Bg>({ -675,500 })->Init("mosshole_environment_1", -3, -2, true);
	GameplayStatics::CreateObject<Bg>({ -725,700 })->Init("mosshole_environment_0", -4, -2, true);
	GameplayStatics::CreateObject<Bg>({ -790,985 })->Init("mosshole_environment_2", -2, -2, true, 1.04f);
	GameplayStatics::CreateObject<Bg>({ -980,845 })->Init("mosshole_environment_3", -3, -2, true, 1.04f);

	GameplayStatics::CreateObject<Bg>({ -100,1060 })->Init("mosshole_floor_0", -4, -2, true);
	GameplayStatics::CreateObject<Bg>({ -575,950 })->Init("mosshole_moss_0", -5, -2, true);
	GameplayStatics::CreateObject<Bg>({ -500,1025 })->Init("mosshole_moss_1", -4, -2, true);
	GameplayStatics::CreateObject<Bg>({ -375,1010 })->Init("mosshole_moss_0", -4, -2, true);
	GameplayStatics::CreateObject<Bg>({ -775,1025 })->Init("mosshole_floor_1", -1, -1, true);
	GameplayStatics::CreateObject<Bg>({ -1050,865 })->Init("mosshole_floor_1", -1, 0, true);
	GameplayStatics::CreateObject<Bg>({ -425,1033 })->Init("mosshole_floor_1", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ -350,1030 })->Init("mosshole_floor_1", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ -250,1035 })->Init("mosshole_floor_1", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ 25,1025 })->Init("mosshole_floor_1", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ 150,1025 })->Init("mosshole_floor_0", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ 350,1030 })->Init("mosshole_floor_0", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ 550,1020 })->Init("mosshole_floor_1", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ -1375,720 })->Init("mosshole_floor_1", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ -1625,730 })->Init("mosshole_floor_0", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ -1700,550 })->Init("mosshole_vine_0", -3, -1, true);
	GameplayStatics::CreateObject<Bg>({ -1500,575 })->Init("mosshole_vine_1", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ -1150,620 })->Init("mosshole_vine_2", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ -250,400 })->Init("mosshole_vine_3", -5, -2, true);
	GameplayStatics::CreateObject<Bg>({ -340,450 })->Init("mosshole_vine_3", -6, -4, true);
	GameplayStatics::CreateObject<Bg>({ 550,385 })->Init("mosshole_vine_3", -6, -4, true);
	GameplayStatics::CreateObject<Bg>({ 660,475 })->Init("mosshole_vine_3", -6, -4, true);
	GameplayStatics::CreateObject<Bg>({ 630,500 })->Init("mosshole_vine_3", 2);


	Animated* waterfall = GameplayStatics::CreateObject<Animated>({ -625,700 }); waterfall->Init("water_stream", 0.05f, -1, 100);
	waterfall = GameplayStatics::CreateObject<Animated>({ -800,800 }, 0, {0.95f, 1.f}); waterfall->Init("water_stream", 0.05f, -1, 100);
	waterfall = GameplayStatics::CreateObject<Animated>({ -1000,700 }, 0, {0.8f, 1.f}); waterfall->Init("water_stream", 0.05f, -1, 80);
	waterfall = GameplayStatics::CreateObject<Animated>({ -1075,650 }, 0, {0.7f, 0.9f}); waterfall->Init("water_stream", 0.05f, -3, 80);

	Plant* grass = GameplayStatics::CreateObject<Plant>({ -225,965 }); grass->Init("grass_3", 0.06f, -1); grass->Init_("grass_3_death", { 75,100 });
	grass = GameplayStatics::CreateObject<Plant>({ -300,950 }); grass->Init("grass_0", 0.06f, -1); grass->Init_("grass_0_death", { 100,100 });
	grass = GameplayStatics::CreateObject<Plant>({ -425,975 }); grass->Init("grass_2", 0.06f, 4);
	grass = GameplayStatics::CreateObject<Plant>({ -675,980 }); grass->Init("grass_1", 0.06f, 4);
	grass = GameplayStatics::CreateObject<Plant>({ -925,798 }, 0, { -1.f, 1.f }); grass->Init("grass_3", 0.06f, -2); grass->Init_("grass_3_death", { 75,100 });
	grass = GameplayStatics::CreateObject<Plant>({ -980,800 }); grass->Init("grass_2", 0.06f, -2); grass->Init_("grass_2_death", { 85,100 });
	grass = GameplayStatics::CreateObject<Plant>({ -1025,790 }); grass->Init("grass_3", 0.06f, -2); grass->Init_("grass_3_death", { 75,100 });
	grass = GameplayStatics::CreateObject<Plant>({ 450,970 }); grass->Init("grass_3", 0.06f, -3);


	GameplayStatics::CreateObject<PlatForm>({ 700,325 })->Init("", { 300,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 800,550 })->Init("", { 200,1000 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -100,1125 })->Init("", { 2000,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1100,975 })->Init("", { 500,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1600,825 })->Init("", { 800,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1400,225 })->Init("", { 800,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -650,275 })->Init("", { 800,300 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 0,50 })->Init("", { 2000,200 }, {});

	for (int i = -3; i < 3; ++i)
	{
		GameplayStatics::CreateObject<Bg>({ i * 280.f, 1035 })->Init("mosshole_ground_0", 1);
	}
	for (int i = -8; i < 7; ++i)
	{
		GameplayStatics::CreateObject<Bg>({ i * 110.f, 1045 })->Init("mosshole_ground_1", 2);
	}
	MossFloor* mossFloor;
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 0.f, 1045.f }); mossFloor->Init("mosshole_floor_0", 3); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 125.f, 1042.f }); mossFloor->Init("mosshole_floor_1", 3); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 225.f, 1040.f }, 0, { -1.f, 1.f }); mossFloor->Init("mosshole_floor_1", 3); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 500.f, 1042.f }, 0, { -1.f, 1.f }); mossFloor->Init("mosshole_floor_1", 3); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 375.f, 1047.f }, 0, { 1.1f, 1.05f }); mossFloor->Init("mosshole_floor_0", 3); mossFloor->Init_({ 175.f,50.f });

	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -500.f, 1045.f }); mossFloor->Init("mosshole_floor_0", 3); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -650.f, 1047.f }, 0, { -1.1f, 1.f }); mossFloor->Init("mosshole_floor_0", 3); mossFloor->Init_({ 175.f,50.f });

	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -980.f, 890.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1100.f, 892.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1200.f, 888.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });

	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1280.f, 736.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1425.f, 740.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1550.f, 742.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1700.f, 738.f }, 0, { -1.f, 1.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });


	GameplayStatics::CreateObject<Bg>({ 610.f, 1025.f })->Init("mosshole_moss_0", 3);
	GameplayStatics::CreateObject<Bg>({ -900.f, 1000.f }, -90.f, { 1.f, -1.f })->Init("mosshole_floor_0", 5);
	GameplayStatics::CreateObject<Bg>({ -990.f, 930.f })->Init("mosshole_moss_0", 6, 1, false, 1.02);
	GameplayStatics::CreateObject<Bg>({ -1125.f, 940.f }, 0.f, { -1.f, 1.f })->Init("mosshole_moss_1", 6, 1, false, 1.02);
	GameplayStatics::CreateObject<Bg>({ -1050.f, 950.f })->Init("mosshole_moss_0", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -1220.f, 920.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -960.f, 1000.f })->Init("mosshole_moss_1", 7, 1);

	for (int i = -4; i < 3; ++i)
	{
		GameplayStatics::CreateObject<Bg>({ i * 225.f, 1051 })->Init("mosshole_ground_2", 4, 1);
	}
	GameplayStatics::CreateObject<Bg>({ 525.f, 1075.f })->Init("mosshole_moss_1", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 430.f, 1085.f })->Init("mosshole_moss_0", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 300.f, 1060.f })->Init("mosshole_moss_1", 5, 1);
	GameplayStatics::CreateObject<Bg>({ -835.f, 1035.f }, 0, { -0.9f, 0.9f })->Init("mosshole_moss_1", 7, 1);
	for (int i = -3; i < 3; ++i)
	{
		GameplayStatics::CreateObject<Bg>({ i * 274.f + 50.f, 1051 })->Init("mosshole_ground_4", 6, 2);
	}
	for (int i = -2; i < 3; ++i)
	{
		GameplayStatics::CreateObject<Bg>({ i * 274.f, 1058 })->Init("mosshole_ground_3", 7, 2);
	}
	GameplayStatics::CreateObject<Bg>({ 350.f, 1065.f })->Init("mosshole_moss_0", 8, 2);
	GameplayStatics::CreateObject<Bg>({ 175.f, 1085.f })->Init("mosshole_moss_0", 8, 2);
	GameplayStatics::CreateObject<Bg>({ 75.f, 1080.f }, 0, {-1.f, 1.f})->Init("mosshole_moss_1", 8, 2);
	GameplayStatics::CreateObject<Bg>({ 690.f, 1025.f })->Init("mosshole_moss_0", 8, 2);

	GameplayStatics::CreateObject<Bg>({ -450.f, 1050.f })->Init("mosshole_moss_0", 8, 2);
	GameplayStatics::CreateObject<Bg>({ -525.f, 1060.f })->Init("mosshole_moss_0", 8, 2);
	GameplayStatics::CreateObject<Bg>({ -625.f, 1045.f }, 0, { -1.f, 1.f })->Init("mosshole_moss_1", 8, 2);

	for (int i = -2; i < 1; ++i)
	{
		GameplayStatics::CreateObject<Bg>({ i * 185.f - 50.f, 1060 })->Init("mosshole_ground_5", 9, 3);
	}
	GameplayStatics::CreateObject<Bg>({ 575.f, 1045.f })->Init("mosshole_moss_2", 10, 3);
	GameplayStatics::CreateObject<Bg>({ 450.f, 1065.f })->Init("mosshole_moss_3", 11, 4);
	GameplayStatics::CreateObject<Bg>({ 325.f, 1070.f }, 0, { -1.f, 1.f })->Init("mosshole_moss_3", 11, 4);
	GameplayStatics::CreateObject<Bg>({ 375.f, 1050.f })->Init("mosshole_moss_2", 12, 5);
	GameplayStatics::CreateObject<Bg>({ 225.f, 1055.f }, 0, { -1.f, 1.f })->Init("mosshole_moss_3", 11, 4);
	GameplayStatics::CreateObject<Bg>({ 100.f, 1085.f })->Init("mosshole_moss_3", 10, 3);
	GameplayStatics::CreateObject<Bg>({ -25.f, 1050.f })->Init("mosshole_moss_2", 10, 3);
	GameplayStatics::CreateObject<Bg>({ -175.f, 1100.f })->Init("mosshole_moss_2", 10, 3);
	GameplayStatics::CreateObject<Bg>({ -275.f, 1075.f })->Init("mosshole_moss_3", 11, 4);
	GameplayStatics::CreateObject<Bg>({ -375.f, 1050.f })->Init("mosshole_moss_2", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -525.f, 1070.f }, 0, { -1.f, 1.f })->Init("mosshole_moss_2", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -450.f, 1025.f })->Init("mosshole_moss_2", 11, 4);
	GameplayStatics::CreateObject<Bg>({ -625.f, 1075.f })->Init("mosshole_moss_2", 10, 3);
	GameplayStatics::CreateObject<Bg>({ -735.f, 1055.f })->Init("mosshole_moss_3", 10, 3);


	GameplayStatics::CreateObject<Bg>({ -875.f, 1015.f }, -90.f)->Init("mosshole_ground_2", 2);
	GameplayStatics::CreateObject<Bg>({ -1038.f, 880.f }, 0, { 1.17f, 1.2f })->Init("mosshole_ground_0", 3);
	GameplayStatics::CreateObject<Bg>({ -1227.f, 865.f }, -90.f)->Init("mosshole_ground_2", 2);
	GameplayStatics::CreateObject<Bg>({ -1245.f, 865.f }, -90.f, { 1.25f, 0.8f })->Init("mosshole_ground_3", 3);
	GameplayStatics::CreateObject<Bg>({ -1365.f, 730.f })->Init("mosshole_ground_0", 3);
	GameplayStatics::CreateObject<Bg>({ -1655.f, 730.f })->Init("mosshole_ground_0", 3);

	GameplayStatics::CreateObject<Bg>({ -1325.f, 790.f })->Init("mosshole_moss_1", 6);
	GameplayStatics::CreateObject<Bg>({ -1400.f, 800.f }, 0.f, { -1.f, 1.f })->Init("mosshole_moss_0", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -1275.f, 850.f })->Init("mosshole_moss_3", 9, 2);
	GameplayStatics::CreateObject<Bg>({ -1220.f, 925.f })->Init("mosshole_moss_3", 10, 2);
	GameplayStatics::CreateObject<Bg>({ -1500.f, 785.f })->Init("mosshole_moss_0", 8, 1);
	GameplayStatics::CreateObject<Bg>({ -1595.f, 800.f })->Init("mosshole_moss_1", 8, 1);
	GameplayStatics::CreateObject<Bg>({ -1675.f, 765.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -1750.f, 775.f })->Init("mosshole_moss_0", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -1700.f, 810.f })->Init("mosshole_moss_2", 9, 2);
	GameplayStatics::CreateObject<Bg>({ -1525.f, 825.f })->Init("mosshole_moss_2", 9, 2);


	GameplayStatics::CreateObject<Bg>({ 645.f, 1055.f })->Init("mosshole_moss_4", 13, 5);
	GameplayStatics::CreateObject<Bg>({ 525.f, 1065.f })->Init("mosshole_moss_4", 13, 5);
	GameplayStatics::CreateObject<Bg>({ 125.f, 1080.f })->Init("mosshole_moss_4", 13, 5);
	GameplayStatics::CreateObject<Bg>({ -325.f, 1085.f })->Init("mosshole_moss_4", 13, 5);
	GameplayStatics::CreateObject<Bg>({ -480.f, 1075.f })->Init("mosshole_moss_4", 13, 5);
	GameplayStatics::CreateObject<Bg>({ -800.f, 1050.f }, 0, { -0.9f, 0.9f })->Init("mosshole_moss_4", 12, 4, 1.03f);
	GameplayStatics::CreateObject<Bg>({ -500.f, 1000.f })->Init("mosshole_fung_new_0105_Layer-254-copy-2", 13, 6);
	GameplayStatics::CreateObject<Bg>({ 0.f, 1000.f })->Init("mosshole_fung_new_0104_Layer-254-copy-3", 13, 6);
	GameplayStatics::CreateObject<Bg>({ 525.f, 1000.f }, 0, { -1.f, 1.f })->Init("mosshole_fung_new_0106_Layer-254-copy", 13, 6);
	GameplayStatics::CreateObject<Bg>({ -920.f, 1025.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1000.f, 980.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1025.f, 1035.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1130.f, 950.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1225.f, 975.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1300.f, 925.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1625.f, 850.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1500.f, 900.f })->Init("mosshole_moss_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1400.f, 800.f })->Init("mosshole_shadow_0", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1050.f, 650.f })->Init("mosshole_shadow_1", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1075.f, 650.f })->Init("mosshole_shadow_2", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1250.f, 610.f })->Init("mosshole_shadow_3", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -500,550 })->Init("mosshole_vine_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -650,550 })->Init("mosshole_vine_4", 12, 6);
	GameplayStatics::CreateObject<Bg>({ -900,450 }, 0, { 1.2f, 1.1f })->Init("mosshole_vine_4", 12, 4);
	GameplayStatics::CreateObject<Bg>({ -1325,350 })->Init("mosshole_vine_4", 12, 6);


	GameplayStatics::CreateObject<Bg>({ -1350.f, 400.f })->Init("mosshole_roof_0", 3);
	GameplayStatics::CreateObject<Bg>({ -675.f, 500.f })->Init("mosshole_roof_1", 3);
	GameplayStatics::CreateObject<Bg>({ -465, 220 })->Init("mosshole_deep_mask", 5);
	GameplayStatics::CreateObject<Bg>({ -1020, 220 })->Init("mosshole_deep_mask", 5);
	GameplayStatics::CreateObject<Bg>({ 950.f, 475.f })->Init("mosshole_roof_1", 3);
	GameplayStatics::CreateObject<Bg>({ 750,900 }, -90.f, { -1.f,1.f })->Init("mosshole_environment_2", 4);
	GameplayStatics::CreateObject<Bg>({ 780,575 }, 90.f, { 1.f, -1.f })->Init("mosshole_environment_3", 3);
	GameplayStatics::CreateObject<Bg>({ 775,975 }, 90.f)->Init("mosshole_environment_0", -4, -3, true);
}

void MossHoleLevel::BeginPlay()
{
	Level::BeginPlay();

	if (!player)
	{
		player = Cast<Player>(GameplayStatics::GetController());
	}

}

void MossHoleLevel::Update(float deltaTime)
{
	Level::Update(deltaTime);

	if (!player)
	{
		player = Cast<Player>(GameplayStatics::GetController());
	}

	Camera* camera = player->GetComponentByClass<Camera>();
	if (player->GetWorldPosition().x > -850)camera->SetRectFrame(FRect({ -1200.f,650.f }, { 200.f,750.f }));
	else if (player->GetWorldPosition().x < -1200)camera->SetRectFrame(FRect({ -1200.f,500.f }, { 200.f,550.f }));
	else camera->SetRectFrame(FRect({ -1200.f,550.f }, { 200.f,650.f }));
}
