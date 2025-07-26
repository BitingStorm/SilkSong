#include "MossHole_0.h"
#include "PlatForm.h"
#include "Bg.h"
#include "MossFloor.h"
#include "Player.h"
#include "GameModeHelper.h"
#include "Components/Camera.h"
#include "Animated.h"
#include "Plant.h"
#include "SceneTitleUI.h"
#include "LevelTransformer.h"
#include "Bug.h"
#include "Fly.h"
#include "Components/SpriteRenderer.h"
#include "BubbleProducer.h"


MossHoleLevel0::MossHoleLevel0()
{
	GameplayStatics::CreateObject<LevelTransformer>({ 2550,900 })->Init("MossHole", { -1775,550 }, FRect({ -1200.f,500.f }, { 200.f,550.f }));

	GameplayStatics::CreateObject<Bg>({ -1600,660 })->Init("mosshole_0", -20, -12);
	GameplayStatics::CreateObject<Bg>({ 950,750 })->Init("mosshole_3", -21, -15);
	GameplayStatics::CreateObject<Bg>({ 2485,875 }, 0, {-1.f, 1.f})->Init("mosshole_cave_light", -2);
	GameplayStatics::CreateObject<Bg>({ 2380,885 })->Init("mosshole_environment_4", -3, -3, true);
	GameplayStatics::CreateObject<Bg>({ 1950,950 })->Init("mosshole_environment_2", -4, -4, true);
	GameplayStatics::CreateObject<Bg>({ 1400,1175 })->Init("mosshole_environment_2", -4, -4, true);
	GameplayStatics::CreateObject<Bg>({ 900,1225 })->Init("mosshole_environment_3", -4, -4, true);
	GameplayStatics::CreateObject<Bg>({ 400,1215 })->Init("mosshole_environment_1", -4, -4, true);
	GameplayStatics::CreateObject<Bg>({ 2000,1000 })->Init("mosshole_moss_1", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ 2100,985 })->Init("mosshole_moss_0", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ 1300,1200 })->Init("mosshole_moss_0", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ 300,1260 })->Init("mosshole_moss_1", -2, -1, true);
	GameplayStatics::CreateObject<Bg>({ -2500,840 })->Init("mosshole_environment_1", -3, -3, true);
	GameplayStatics::CreateObject<Bg>({ -2450,550 })->Init("mosshole_environment_0", -4, -4, true);
	GameplayStatics::CreateObject<Bg>({ -2420,350 })->Init("mosshole_environment_2", -5, -4, true);
	GameplayStatics::CreateObject<Bg>({ -1830.f, 175.f })->Init("mosshole_roof_1", -3, -3, true);
	GameplayStatics::CreateObject<Bg>({ -1000.f, 225.f })->Init("mosshole_roof_1", -3, -3, true);
	GameplayStatics::CreateObject<Bg>({ -350,625 })->Init("mosshole_environment_5", -5, -5);


	/* об╡Ц */
	GameplayStatics::CreateObject<PlatForm>({ 2000,1200 })->Init("", { 1000,400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1150,1325 })->Init("", { 700,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 625,1250 })->Init("", { 350,300 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 350,1375 })->Init("", { 300,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 0,1200 })->Init("", { 500,400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -450,900 })->Init("", { 400,400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -900,1075 })->Init("", { 500,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1400,900 })->Init("mosshole_plat_7", { 90,50 }, { 0,-15 });
	GameplayStatics::CreateObject<PlatForm>({ -1700,880 })->Init("mosshole_plat_8", { 100,50 }, { 0,-10 });
	GameplayStatics::CreateObject<PlatForm>({ -2300,850 })->Init("", { 800,50 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1900,1185 })->Init("", { 1500,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -2630,600 })->Init("", { 200,1000 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -2405,200 })->Init("", { 250,400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -800,75 })->Init("", { 500,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -175,250 })->Init("", { 1030,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 850,310 })->Init("", { 1030,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1750,325 })->Init("", { 1030,200 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 2500,500 })->Init("", { 750,200 }, {});


	MossFloor* mossFloor;
	Plant* grass;

	//Platform1
	for (int i = -2; i < 4; ++i)
	{
		mossFloor = GameplayStatics::CreateObject<MossFloor>({ i * 175.f + 1945.f + FMath::RandReal(0.f, 5.f), 1010.f + FMath::RandReal(0.f, 5.f) },
			0.f, { FMath::RandPerc() > 0.5 ? 1.f : -1.f, 1.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	}
	GameplayStatics::CreateObject<Bg>({ 1510.f, 1155.f }, 90.f)->Init("mosshole_ground_2", 2);
	GameplayStatics::CreateObject<Bg>({ 1535.f, 1170.f }, 90.f)->Init("mosshole_ground_3", 3);
	GameplayStatics::CreateObject<Bg>({ 1690.f, 1070.f })->Init("mosshole_ground_6", 3);


	GameplayStatics::CreateObject<Bg>({ 2035.f, 1046.f }, 0.f, { -1.f, 1.f })->Init("mosshole_moss_1", 6, 1);
	GameplayStatics::CreateObject<Bg>({ 2125.f, 1050.f })->Init("mosshole_moss_0", 6, 1);
	GameplayStatics::CreateObject<Bg>({ 2225.f, 1043.f })->Init("mosshole_moss_1", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 1860.f, 1038.f })->Init("mosshole_ground_6", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 1750.f, 1043.f })->Init("mosshole_moss_1", 6, 1);
	GameplayStatics::CreateObject<Bg>({ 1660.f, 1060.f })->Init("mosshole_moss_0", 7, 1);
	GameplayStatics::CreateObject<Bg>({ 1585.f, 1075.f }, 0.f, { -1.f, 1.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ 1570.f, 1165.f })->Init("mosshole_moss_0", 8, 1);
	GameplayStatics::CreateObject<Bg>({ 1475.f, 1235.f })->Init("mosshole_moss_0", 9, 1);
	GameplayStatics::CreateObject<Bg>({ 1675.f, 1140.f }, 0.f, { -1.1f, 1.1f })->Init("mosshole_moss_2", 9, 2);
	GameplayStatics::CreateObject<Bg>({ 1600.f, 1200.f })->Init("mosshole_moss_3", 10, 2);
	GameplayStatics::CreateObject<Bg>({ 1525.f, 1230.f })->Init("mosshole_moss_4", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 2350.f, 1080.f })->Init("mosshole_moss_4", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 1900.f, 1080.f })->Init("mosshole_moss_3", 7, 2);
	GameplayStatics::CreateObject<Bg>({ 1800.f, 1095.f }, 0.f, { -1.f, 1.f })->Init("mosshole_moss_3", 7, 2);
	GameplayStatics::CreateObject<Bg>({ 2330.f, 1040.f })->Init("mosshole_moss_2", 7, 1);
	GameplayStatics::CreateObject<Bg>({ 2430.f, 1060.f })->Init("mosshole_moss_3", 6, 1);
	GameplayStatics::CreateObject<Bg>({ 2250.f, 1090.f }, 0.f, { -1.f, 1.f })->Init("mosshole_moss_3", 8, 2);
	GameplayStatics::CreateObject<Bg>({ 2150.f, 1075.f })->Init("mosshole_moss_2", 7, 2);
	GameplayStatics::CreateObject<Bg>({ 2000.f, 1095.f })->Init("mosshole_moss_2", 8, 2);
	GameplayStatics::CreateObject<Bg>({ 2075.f, 1130.f })->Init("mosshole_moss_4", 9, 3);
	GameplayStatics::CreateObject<Bg>({ 1800.f, 1270.f })->Init("mosshole_shadow_4", 11, 3);

	grass = GameplayStatics::CreateObject<Plant>({ 1650,935 }); grass->Init("grass_3", 0.06f, -1); grass->Init_("grass_3_death", { 75,100 });
	grass = GameplayStatics::CreateObject<Plant>({ 1725,920 }); grass->Init("grass_0", 0.06f, -1); grass->Init_("grass_0_death", { 100,100 });


	//Platform2
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 1390.f, 1245.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 1275.f, 1245.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 1100.f, 1240.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 975.f, 1242.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 880.f, 1240.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	GameplayStatics::CreateObject<Bg>({ 850.f, 1250.f })->Init("mosshole_moss_2", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 1000.f, 1275.f })->Init("mosshole_moss_3", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 1125.f, 1275.f }, 0, {1.2f, 1.f})->Init("mosshole_moss_3", 7, 1);
	GameplayStatics::CreateObject<Bg>({ 1250.f, 1287.f })->Init("mosshole_moss_2", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 1350.f, 1275.f })->Init("mosshole_moss_2", 10, 1);
	GameplayStatics::CreateObject<PlatForm>({ 1190,990 })->Init("mosshole_plat_6", { 275,50 }, {});
	grass = GameplayStatics::CreateObject<Plant>({ 1250,1160 }); grass->Init("grass_1", 0.06f, -1); grass->Init_("grass_1_death", { 150,100 });
	grass = GameplayStatics::CreateObject<Plant>({ 1000,1150 }); grass->Init("grass_3", 0.06f, -1); grass->Init_("grass_3_death", { 75,100 });
	GameplayStatics::CreateObject<Bug>({ 1100,1100 });

	//Platform3
	GameplayStatics::CreateObject<Bg>({ 460.f, 1235.f }, 90.f)->Init("mosshole_ground_2", 2);
	GameplayStatics::CreateObject<Bg>({ 485.f, 1250.f }, 90.f)->Init("mosshole_ground_3", 3);
	GameplayStatics::CreateObject<Bg>({ 785.f, 1250.f }, -90.f)->Init("mosshole_ground_6", 3);
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 550.f, 1110.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 700.f, 1110.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	GameplayStatics::CreateObject<Bg>({ 525.f, 1160.f })->Init("mosshole_moss_0", 6, 1);
	GameplayStatics::CreateObject<Bg>({ 625.f, 1130.f })->Init("mosshole_moss_1", 5, 1);
	GameplayStatics::CreateObject<Bg>({ 735.f, 1165.f }, 0, { -1.f, 1.f })->Init("mosshole_moss_0", 6, 1);
	GameplayStatics::CreateObject<Bg>({ 600.f, 1190.f })->Init("mosshole_moss_2", 8, 2);
	GameplayStatics::CreateObject<Bg>({ 550.f, 1350.f })->Init("mosshole_shadow_4", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 750.f, 1210.f })->Init("mosshole_moss_4", 9, 3);
	grass = GameplayStatics::CreateObject<Plant>({ 775,1020 }, 0.f, { -1.f, 1.f }); grass->Init("grass_3", 0.06f, -1); grass->Init_("grass_3_death", { 75,100 });

	//Platform4 ~ 5
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 330.f, 1295.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });

	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 180.f, 1020.f }); mossFloor->Init("mosshole_floor_1", 3); mossFloor->Init_({ 100.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ 50.f, 1020.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -130.f, 1020.f }, 0, {-1.f, 1.f}); mossFloor->Init("mosshole_floor_2", 4); mossFloor->Init_({ 175.f,50.f });

	GameplayStatics::CreateObject<Bg>({ 235.f, 1105.f }, -90.f)->Init("mosshole_floor_0", 4);
	GameplayStatics::CreateObject<Bg>({ 230.f, 1225.f }, -30.f)->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ 170.f, 1100.f }, 15.f, {-1.f, 1.f})->Init("mosshole_moss_1", 5);
	GameplayStatics::CreateObject<Bg>({ 75.f, 1070.f })->Init("mosshole_moss_0", 6);
	GameplayStatics::CreateObject<Bg>({ 0.f, 1060.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -100.f, 1080.f })->Init("mosshole_moss_0", 8, 1);
	GameplayStatics::CreateObject<Bg>({ -180.f, 1050.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ 124.f, 1215.f })->Init("mosshole_shadow_5", 11, 2);
	GameplayStatics::CreateObject<Bg>({ 25.f, 1140.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -125.f, 1120.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -250.f, 1085.f }, 10.f, {-1.f, 1.f})->Init("mosshole_moss_4", 11, 2);
	grass = GameplayStatics::CreateObject<Plant>({ -75,940 }); grass->Init("grass_2", 0.06f, -1); grass->Init_("grass_2_death", { 80,100 });
	GameplayStatics::CreateObject<Bug>({ 0,900 });

	//Platform6
	GameplayStatics::CreateObject<Bg>({ -260.f, 835.f }, -90.f)->Init("mosshole_ground_2", 2);
	GameplayStatics::CreateObject<Bg>({ -278.f, 875.f }, -90.f)->Init("mosshole_ground_3", 3);
	GameplayStatics::CreateObject<Bg>({ -650.f, 850.f }, 90.f)->Init("mosshole_ground_6", 3);
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -550.f, 710.f }, 0.f, {-1.f, 1.f}); mossFloor->Init("mosshole_floor_2", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -450.f, 713.f }); mossFloor->Init("mosshole_floor_1", 5); mossFloor->Init_({ 75.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -350.f, 710.f }); mossFloor->Init("mosshole_floor_2", 4); mossFloor->Init_({ 175.f,50.f });
	GameplayStatics::CreateObject<Bg>({ -580.f, 765.f }, 0.f, {-1.f,1.f})->Init("mosshole_moss_1", 6);
	GameplayStatics::CreateObject<Bg>({ -500.f, 790.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -410.f, 770.f })->Init("mosshole_moss_0", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -330.f, 780.f })->Init("mosshole_moss_1", 6);
	GameplayStatics::CreateObject<Bg>({ -600.f, 865.f })->Init("mosshole_moss_2", 9, 2, false, 1.03f);
	GameplayStatics::CreateObject<Bg>({ -435.f, 835.f })->Init("mosshole_moss_2", 9, 2, false, 1.035f);
	GameplayStatics::CreateObject<Bg>({ -300.f, 870.f }, -25.f)->Init("mosshole_moss_3", 9, 2, false, 1.03f);
	GameplayStatics::CreateObject<Bg>({ -250.f, 980.f }, -45.f)->Init("mosshole_moss_3", 9, 2, false, 1.035f);

	GameplayStatics::CreateObject<Bg>({ -450.f, 1000.f })->Init("mosshole_shadow_4", 11, 2);


	//Platform7
	GameplayStatics::CreateObject<Bg>({ -1145.f, 1115.f }, 90.f)->Init("mosshole_ground_2", 2);
	GameplayStatics::CreateObject<Bg>({ -1130.f, 1130.f }, 90.f)->Init("mosshole_ground_6", 3);
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -800.f, 985.f }, 0.f, {-1.f, 1.f}); mossFloor->Init("mosshole_floor_2", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -990.f, 985.f }); mossFloor->Init("mosshole_floor_2", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -1110.f, 990.f }); mossFloor->Init("mosshole_floor_1", 4); mossFloor->Init_({ 100.f,50.f });
	GameplayStatics::CreateObject<Bg>({ -685.f, 980.f })->Init("mosshole_moss_2", 9, 2, false, 1.03f);
	GameplayStatics::CreateObject<Bg>({ -830.f, 1030.f })->Init("mosshole_moss_0", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -950.f, 1025.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -1075.f, 1036.f })->Init("mosshole_moss_1", 7, 1);

	GameplayStatics::CreateObject<Bg>({ -700.f, 1030.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -850.f, 1040.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -1000.f, 1050.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -1100.f, 1080.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -800.f, 1190.f })->Init("mosshole_fung_mask_01", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -450.f, 1125.f })->Init("mosshole_deep_mask", 10, 2);
	GameplayStatics::CreateObject<Bug>({ -800,850 });
	
	//Platform8
	Bg* bg;
	bg = GameplayStatics::CreateObject<Bg>({ -1700, 1160 }); bg ->Init("mosshole_lake", 2); bg->GetComponentByClass<SpriteRenderer>()->SetTransparency(210.f);
	bg = GameplayStatics::CreateObject<Bg>({ -1700, 1140 }); bg->Init("mosshole_lake", 5); bg->GetComponentByClass<SpriteRenderer>()->SetTransparency(210.f);
	bg = GameplayStatics::CreateObject<Bg>({ -1700, 1190 }); bg->Init("mosshole_lake", 1);
	GameplayStatics::CreateObject<Bg>({ -1700.f, 950.f })->Init("mosshole_lake_light", 1);

	Animated* lakeTop;
	for (int i = -2; i < 2; ++i)
	{
		lakeTop = GameplayStatics::CreateObject<Animated>({ -1680 + i * 325.f + FMath::RandReal(-5.f, 5.f), 1050 + FMath::RandReal(-2.f, 2.f) });
		lakeTop->Init("lake_top", 0.045f, 6, 150);
	}
	lakeTop = GameplayStatics::CreateObject<Animated>({ -1150, 1070 }); lakeTop->Init("lake_top", 0.04f, 6, 150);

	grass = GameplayStatics::CreateObject<Plant>({ -1700,770 }); grass->Init("grass_3", 0.06f, -1); grass->Init_("grass_3_death", { 75,100 });
	grass = GameplayStatics::CreateObject<Plant>({ -2200,760 }, 0.f, {-1.f,1.f}); grass->Init("grass_3", 0.06f, -1); grass->Init_("grass_3_death", { 75,100 });
	grass = GameplayStatics::CreateObject<Plant>({ -2350,760 }); grass->Init("grass_1", 0.06f, -1); grass->Init_("grass_1_death", { 150,100 });


	GameplayStatics::CreateObject<Bg>({ -2450.f, 1050.f })->Init("mosshole_fung_mask_04", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -2475.f, 450.f })->Init("mosshole_msk_angle_stalactites", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -2300.f, 860.f })->Init("mosshole_Slug_Shrine_0001_a", 2);
	GameplayStatics::CreateObject<Bg>({ -2450.f, 250.f })->Init("mosshole_fung_mask_02", 11, 2);


	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -2380.f, 835.f }); mossFloor->Init("mosshole_floor_0", 4); mossFloor->Init_({ 175.f,50.f });
	mossFloor = GameplayStatics::CreateObject<MossFloor>({ -2565.f, 835.f }); mossFloor->Init("mosshole_floor_2", 4); mossFloor->Init_({ 175.f,50.f });
	GameplayStatics::CreateObject<Bg>({ -2260.f, 980.f }, -45.f)->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -2305.f, 900.f }, -15.f)->Init("mosshole_moss_3", 8, 1);
	GameplayStatics::CreateObject<Bg>({ -2390.f, 875.f })->Init("mosshole_moss_1", 7, 1);
	GameplayStatics::CreateObject<Bg>({ -2450.f, 895.f })->Init("mosshole_moss_2", 8, 1);
	GameplayStatics::CreateObject<Bg>({ -2550.f, 860.f })->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -2550.f, 730.f }, -75.f)->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -2510.f, 640.f }, -160.f)->Init("mosshole_moss_4", 11, 2);
	GameplayStatics::CreateObject<Bg>({ -2520.f, 760.f }, -60.f)->Init("mosshole_moss_3", 8, 1);

	GameplayStatics::CreateObject<PlatForm>({ -2000,600 })->Init("mosshole_plat_9", { 150,50 }, { 0,-15 });
	GameplayStatics::CreateObject<PlatForm>({ -1800,400 })->Init("mosshole_plat_7", { 90,50 }, { 0,-15 });
	GameplayStatics::CreateObject<PlatForm>({ -2000,200 })->Init("mosshole_plat_8", { 100,50 }, { 0,-15 });

	GameplayStatics::CreateObject<Fly>({ -1000,600 });
	GameplayStatics::CreateObject<Fly>({ -2200,500 }, 0.f, {-1.f, 1.f});
	//Platform9
	//.....


	//Others
	GameplayStatics::CreateObject<BubbleProducer>({ -1800,1200 });
	GameplayStatics::CreateObject<Bg>({ 2550.f, 675.f })->Init("mosshole_roof_1", 4);
	GameplayStatics::CreateObject<Bg>({ 1750.f, 500.f })->Init("mosshole_roof_0", 3);
	GameplayStatics::CreateObject<Bg>({ 850.f, 485.f })->Init("mosshole_roof_0", 3);
	GameplayStatics::CreateObject<Bg>({ -175.f, 425.f })->Init("mosshole_roof_0", 3);
	GameplayStatics::CreateObject<Bg>({ 2500.f, 325.f })->Init("mosshole_fung_mask_01", 11, 2);
	GameplayStatics::CreateObject<Bg>({ 2030.f, 600.f })->Init("mosshole_shadow_2", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 2170.f, 830.f })->Init("mosshole_shadow_3", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 2000.f, 480.f })->Init("mosshole_shadow_6", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 250.f, 430.f })->Init("mosshole_shadow_6", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 1950.f, 1040.f }, 0, { -1.f,1.f })->Init("mosshole_fung_new_0106_Layer-254-copy", 12, 4);
	GameplayStatics::CreateObject<Bg>({ 1800.f, 1060.f })->Init("mosshole_fung_new_0105_Layer-254-copy-2", 12, 4);
	GameplayStatics::CreateObject<Bg>({ 1250.f, 1150.f })->Init("mosshole_fung_new_0106_Layer-254-copy", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 900.f, 1150.f })->Init("mosshole_fung_new_0104_Layer-254-copy-3", 11, 3);
	GameplayStatics::CreateObject<Bg>({ 100.f, 1030.f })->Init("mosshole_shadow_0", 11, 3);
	GameplayStatics::CreateObject<Bg>({ -800.f, 900.f })->Init("mosshole_fung_new_0106_Layer-254-copy", 11, 3);
	GameplayStatics::CreateObject<Bg>({ -1100.f, 980.f }, 0, { -1.f,1.f })->Init("mosshole_fung_new_0105_Layer-254-copy-2", 11, 3);
	GameplayStatics::CreateObject<Bg>({ -2020.f, 1070.f })->Init("mosshole_fung_new_0105_Layer-254-copy-2", 11, 3);
	GameplayStatics::CreateObject<Bg>({ -2350.f, 810.f }, 30, { -1.f,1.f })->Init("mosshole_fung_new_0104_Layer-254-copy-3", 11, 3);
}

void MossHoleLevel0::BeginPlay()
{
	Level::BeginPlay();

	if (!player)
	{
		player = Cast<Player>(GameplayStatics::GetController());
	}

	OnLevelDelete.AddLambda([]() {
		GameMode* gameMode = GameModeHelper::GetInstance();
		gameMode->GetAudioPlayer(0)->Stop("stream");
		gameMode->GetAudioPlayer(0)->Stop("cicadachirp");
		gameMode->GetAudioPlayer(0)->Stop("mossgrotto");
		});
}

void MossHoleLevel0::Update(float deltaTime)
{
	Level::Update(deltaTime);


	if (!player)
	{
		player = Cast<Player>(GameplayStatics::GetController());
	}

	if (!ui && player->GetWorldPosition().x < 1800)
	{
		ui = GameplayStatics::CreateUI<SceneTitleUI>();
		ui->AddToViewport();
		GameModeHelper::PlayBGMusic("mossgrotto");
	}

	Camera* camera = player->GetComponentByClass<Camera>();
	if (player->GetWorldPosition().x > 1600)camera->SetRectFrame(FRect({ -2000.f,745.f }, { 1900.f,750.f }));
	else if (player->GetWorldPosition().x > -800)camera->SetRectFrame(FRect({ -2000.f,745.f }, { 1900.f,900.f }));
	else camera->SetRectFrame(FRect({ -2000.f,500.f }, { 1900.f,800.f }));
}
