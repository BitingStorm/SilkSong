#include "ResourceManager.h"
#include <easyx.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")


//************************************************
//*********       用户需编辑部分          **********

void ResourceManager::Initialize()
{
	/*************
	 * 图像资源加载 
	 *************/

	/** 动画 **/
	//Player
	Load("player_idle", "Asset/Animations/Player/Idle/", 139, 164, 6);
	Load("player_walk", "Asset/Animations/Player/Walk/", 124, 164, 8);
	Load("player_walkstart", "Asset/Animations/Player/WalkStart/", { {149,167},{149,129},{146,129},{148,134},{152,146},{144,156} });
	Load("player_walkend", "Asset/Animations/Player/WalkEnd/", { {147,160},{137,160},{147,160} });
	Load("player_rush", "Asset/Animations/Player/Rush/", { {133,130},{135,118},{137,118},{132,114},{140,108},{137,117},{143,119},{138,116},{137,114} });
	Load("player_turn", "Asset/Animations/Player/Turn/", { {141,162},{104,178},{141,162} });
	Load("player_jump", "Asset/Animations/Player/Jump/", { {124,160},{120,172},{116,172},{104,168},{108,168},{143,163} });
	Load("player_rushjump", "Asset/Animations/Player/RushJump/", { {171,115},{204,144},{198,180},{186,183},{186,192},{195,195},
		{198,183},{124,182},{140,171},{146,168},{198,180},{104,168},{108,168},{143,163} });
	Load("player_fall", "Asset/Animations/Player/Fall/", 132, 168, 4);
	Load("player_softland", "Asset/Animations/Player/SoftLand/", 152, 166, 3);
	Load("player_hardland", "Asset/Animations/Player/HardLand/", 164, 152, 4);
	Load("player_attack_0", "Asset/Animations/Player/Attack_0/", { {122,162},{364,183},{321,178},{300,178},{174,144} });
	Load("player_attack_1", "Asset/Animations/Player/Attack_1/", { {122,162},{315,156},{321,159},{321,159},{174,144} });
	Load("player_attackup", "Asset/Animations/Player/AttackUp/", { {160,188},{184,389},{184,222},{184,269},{152,168} });
	Load("player_attackdown", "Asset/Animations/Player/AttackDown/", { {111,193},{103,196},{250,263},{225,225},{192,143} });
	Load("player_rushattack", "Asset/Animations/Player/RushAttack/", { {166,156},{155,176},{180,176},{300,164},{339,164},{400,120},{400,121},{109,121},{174,148} });
	Load("player_attackbounce", "Asset/Animations/Player/AttackBounce/", { {160,160},{170,156},{170,158},{176,160},{171,157},{163,160},{144,153},{114,156},{139,145},{176,138},{183,114} });
	Load("player_evade", "Asset/Animations/Player/Evade/", { {141,166},{141,166},{141,166},{148,160},{148,160},{148,160},{152,166} });
	Load("player_dash", "Asset/Animations/Player/Dash/", { {210,147},{210,148},{192,131},{192,131},{182,131},{182,131},{184,131},{225,160},{225,160} });
	Load("player_airdash", "Asset/Animations/Player/AirDash/", { {182,112},{148,140},{148,146},{178,113},{139,145},{176,138},{185,131},{172,113} });
	Load("player_cure", "Asset/Animations/Player/Cure/", { {343,252},{565,570},{580,490},{450,495},{451,495},{451,216},{155,250},{114,225},
		{433,360},{444,368},{963,660},{1040,600},{1035,600},{1025,520} });
	Load("player_hurt", "Asset/Animations/Player/Hurt/", 228, 172, 6);
	Load("player_throw", "Asset/Animations/Player/Throw/", { {127,170}, {127,170}, {127,170}, {127,170}, {104,142}, {104,142}, {104,142}, {104,142}, {177,163},{177,163},{177,163},{177,163} });
	Load("player_grab", "Asset/Animations/Player/Grab/", { {140,180}, {141,198}, {142,194}, {138,196}, {170,134}, {160,110}, {170,120}, {184,132}, {174,166} });
	Load("player__closeskill", "Asset/Animations/Player/_CloseDisSkill/", 179, 152, 3);
	Load("player_closeskill", "Asset/Animations/Player/CloseDisSkill/", 179, 152, 7);
	Load("player_remoteskill", "Asset/Animations/Player/RemoteDisSkill/", { {215,162},{215,162},{215,162},{215,162},{215,162},{215,162},{215,162},{215,162},
		{118,141},{118,141},{118,141},{118,141},{118,141},{118,141},{185,142},{185,142},{185,142},{185,142},{185,142},{185,142} });
	Load("player_rapidskill", "Asset/Animations/Player/RapidSkill/", { {166,156},{177,148},{109,121},{103,133},{183,144},{174,148} });
	Load("player_lowhealth", "Asset/Animations/Player/LowHealth/", 164, 164, 6);
	Load("player_die", "Asset/Animations/Player/Die/", 146, 119, 4);
	Load("player_sitdown", "Asset/Animations/Player/SitDown.png", 170, 159, 1, 1, 1);
	Load("player_standup", "Asset/Animations/Player/StandUp.png", 177, 186, 1, 1, 1);
	Load("player_lookdown", "Asset/Animations/Player/LookDown.png", 142, 169, 1, 1, 1);
	Load("player_lookup", "Asset/Animations/Player/LookUp.png", 149, 122, 1, 1, 1);
	Load("player_leavestart", "Asset/Animations/Player/LeaveStart/", 304, 199, 3);
	Load("player_leave", "Asset/Animations/Player/Leave/", 304, 199, 5);
	Load("player_leaveend", "Asset/Animations/Player/LeaveEnd/", { {208,102}, {174,148} });
	Load("player_wall", "Asset/Animations/Player/Wall.png", 100, 175, 1, 1, 1);
	Load("player_defend", "Asset/Animations/Player/Defend/", 158, 120, 4);
	Load("player_defendstart", "Asset/Animations/Player/DefendStart/", 150, 172, 3);
	Load("player_defendend", "Asset/Animations/Player/DefendEnd/", 92, 170, 2);
	Load("player_defendattack", "Asset/Animations/Player/DefendAttack/", { {201,190}, {201,190}, {201,190}, {455,320}, {455,300}, {230,147}, {117,147} });
	Load("player_scare", "Asset/Animations/Player/Scare.png", 164, 164, 1, 1, 1);

	//Effect
	Load("effect_dash", "Asset/Animations/Player/DashEffect/", 342, 433, 5);
	Load("effect_dash_", "Asset/Animations/Player/DashEffect_.png", 1280, 856, 6, 3, 2);
	Load("effect_hurt", "Asset/Animations/Player/HurtEffect/", 1400, 470, 4);
	Load("effect_hurt_", "Asset/Animations/Player/HurtEffect_/", 308, 270, 7);
	Load("effect_wetland", "Asset/Animations/Player/WetLandEffect/", 115, 34, 5);
	Load("effect_wetwalk", "Asset/Animations/Player/WetWalkEffect/whitesplash.png", 250, 50, 5, 1, 5);
	Load("effect_darthit", "Asset/Animations/Player/DartHitEffect.png", 348, 215, 4, 1, 4);
	Load("effect_nailhit", "Asset/Animations/Player/NailHitEffect/", { {425,240},{495,290},{568,283} });
	Load("effect_attack", "Asset/Animations/Player/AttackEffect/", { {630,52},{532,55},{338,34},{138,17} });
	Load("effect_attack_", "Asset/Animations/Player/AttackEffect_/", 375, 160, 3);
	Load("effect_counter", "Asset/Animations/Player/CounterEffect/", { {209,167},{397,287},{445,260} });
	Load("effect_closeskill", "Asset/Animations/Player/CloseSkillEffect/", 425, 450, 9);
	Load("effect_throw", "Asset/Animations/Player/ThrowEffect/", 278, 133, 3);
	Load("effect_remoteskill", "Asset/Animations/Player/RemoteSkillEffect/", 900, 40, 6);
	Load("effect_rapidskill", "Asset/Animations/Player/RapidSkillEffect/", { {267,127},{373,161},{354,161} });
	Load("effect_geo", "Asset/Animations/Player/GeoEffect/", { {50,24},{39,32},{81,26},{100,30} });
	Load("effect_sit", "Asset/Animations/Player/SitEffect/", 1059, 500, 4);
	Load("effect_leave", "Asset/Animations/Player/LeaveEffect/", 440, 115, 7);
	Load("effect_death", "Asset/Animations/Enemy/DeathHurtEffect/", { {300,250},{450, 375},{450,375} });
	Load("effect_soulmaster_quake", "Asset/Animations/Enemy/SoulMaster/QuakeEffect/", 750, 315, 5);
	Load("effect_soulmaster_quake_", "Asset/Animations/Enemy/SoulMaster/QuakeEffect_.png", 900, 600, 6, 1, 6);
	Load("effect_soulmaster_teleport", "Asset/Animations/Enemy/SoulMaster/TeleportEffect/", { {385,425},{695,1855},{745,2470},{740,2150} });
	Load("effect_soulorb", "Asset/Animations/Enemy/SoulOrb.png", 518, 60, 7, 1, 7);
	Load("effect_puff", "Asset/Animations/Enemy/OrangePuff.png", 738, 77, 9, 1, 9);
	Load("effect_splat", "Asset/Animations/Enemy/DeathSplat/", 430, 345, 7);
	Load("effect_soulburst", "Asset/Animations/Enemy/SoulBurst/", 222, 222, 4);
	Load("effect_soulspawn", "Asset/Animations/Enemy/SoulSpawn/", 222, 222, 4);
	Load("effect_pillar", "Asset/Animations/Enemy/NightMare/Pillar/", 203, 710, 6);
	Load("effect_flame_particle", "Asset/Animations/Enemy/NightMare/Grimm_flame_ball_particle.png", 51, 17, 3, 1, 3);
	Load("effect_nightmare_cast", "Asset/Animations/Enemy/NightMare/CastEffect/", { {154,170},{278,742},{298,988},{296,860} });
	Load("effect_fireball", "Asset/Animations/Enemy/NightMare/FireBall/", 65, 65, 8);

	//UI
	Load("inventory_bloodidle", "Asset/Animations/UI/BloodIdle/", 33, 49, 6);
	Load("inventory_bloodload", "Asset/Animations/UI/BloodLoad/", 33, 49, 4);
	Load("inventory_bloodminus", "Asset/Animations/UI/BloodMinus/", { {68,136}, {65,140}, {67,131}, {114,128}, {108,128}, {118,112}, {33,47} });
	Load("inventory_silk", "Asset/Animations/UI/Silk/", { {19,75}, {19,75}, {23,71}, {21,68}, {28,91}, {25,47}, {16,39} });
	Load("inventory_soul", "Asset/Animations/UI/Soul/", { {47,48}, {47,48}, {60,57}, {72,67}, {120,120}, {77,77} });
	Load("menu_warning", "Asset/Animations/UI/Warning/", 767, 64, 7);

	//NPC
	Load("brumm", "Asset/Animations/Brumm - atlas0 #392009.png", 1352, 472, 11, 2, 6);

	//Enemy
	Load("fly_idle", "Asset/Animations/Enemy/Fly/Idle/", 120, 135, 5);
	Load("fly_turn", "Asset/Animations/Enemy/Fly/Turn/", 120, 151, 2);
	Load("fly_die", "Asset/Animations/Enemy/Fly/Die/", 142, 108, 3);
	Load("fly_startchase", "Asset/Animations/Enemy/Fly/StartChase/", 149, 151, 4);
	Load("fly_chase", "Asset/Animations/Enemy/Fly/Chase/", 146, 134, 4);
	Load("bug_walk", "Asset/Animations/Enemy/Bug/Walk/", 90, 84, 4);
	Load("bug_turn", "Asset/Animations/Enemy/Bug/Turn/", 87, 82, 3);
	Load("bug_appear", "Asset/Animations/Enemy/Bug/Appear/", 109, 110, 5);
	Load("bug_bury", "Asset/Animations/Enemy/Bug/Bury/", 106, 98, 5);
	Load("bug_die", "Asset/Animations/Enemy/Bug/Death/", 86, 84, 6);
	Load("soulmaster_idle", "Asset/Animations/Enemy/SoulMaster/Idle/", 334, 285, 6);
	Load("soulmaster_turn", "Asset/Animations/Enemy/SoulMaster/Turn/", 334, 285, 2);
	Load("soulmaster_teleport", "Asset/Animations/Enemy/SoulMaster/Teleport/", 405, 466, 7);
	Load("soulmaster_startsummon", "Asset/Animations/Enemy/SoulMaster/StartSummon/", 404, 288, 3);
	Load("soulmaster_summon", "Asset/Animations/Enemy/SoulMaster/Summon/", 405, 290, 4);
	Load("soulmaster_startquake", "Asset/Animations/Enemy/SoulMaster/StartQuake.png", 1682, 614, 8, 2, 4);
	Load("soulmaster_quake", "Asset/Animations/Enemy/SoulMaster/Quake.png", 199, 356, 1, 1, 1);
	Load("soulmaster_startdash", "Asset/Animations/Enemy/SoulMaster/StartDash/", 384, 305, 6);
	Load("soulmaster_dash", "Asset/Animations/Enemy/SoulMaster/Dash/", 351, 275, 3);
	Load("soulmaster_startstun", "Asset/Animations/Enemy/SoulMaster/StartStun/", 293, 270, 10);
	Load("soulmaster_stun", "Asset/Animations/Enemy/SoulMaster/Stun/", 125, 262, 6);
	Load("soulmaster_transition", "Asset/Animations/Enemy/SoulMaster/Transition/", 180, 246, 3);
	Load("soulmaster_die", "Asset/Animations/Enemy/SoulMaster/Die/", { {226,146}, {226,146}, {226,146}, {260,135}, {260,135}, {260,135}, {260,135} });
	Load("nightmare_idle", "Asset/Animations/Enemy/NightMare/Idle/", 161, 320, 12);
	Load("nightmare_bow", "Asset/Animations/Enemy/NightMare/Bow/", 200, 324, 7);
	Load("nightmare_teleport", "Asset/Animations/Enemy/NightMare/Teleport/", 188, 337, 7);
	Load("nightmare_startspike", "Asset/Animations/Enemy/NightMare/StartSpike/", 274, 324, 7);
	Load("nightmare_spike", "Asset/Animations/Enemy/NightMare/Spike/", 274, 324, 3);
	Load("nightmare_startballoon", "Asset/Animations/Enemy/NightMare/StartBalloon/", { {196,239}, {321,352} });
	Load("nightmare_balloon", "Asset/Animations/Enemy/NightMare/Balloon/", 321, 352, 3);
	Load("nightmare_cast", "Asset/Animations/Enemy/NightMare/Cast/", { {204,336}, {204,336}, {204,336}, {204,336}, {341,316}, {341,316}, {341,316}, {341,316} });
	Load("nightmare_startairdash", "Asset/Animations/Enemy/NightMare/StartAirDash/", 217, 345, 7);
	Load("nightmare_airdash", "Asset/Animations/Enemy/NightMare/AirDash/", 137, 336, 3);
	Load("nightmare_startdash", "Asset/Animations/Enemy/NightMare/StartDash/", 330, 246, 4);
	Load("nightmare_dash", "Asset/Animations/Enemy/NightMare/Dash/", 569, 208, 3);
	Load("nightmare_startslash", "Asset/Animations/Enemy/NightMare/StartSlash/", 226, 296, 4);
	Load("nightmare_slash", "Asset/Animations/Enemy/NightMare/Slash/", { {506,280}, {418,294}, {495,274}, {380,306}, {380,306}, {380,306} });
	Load("nightmare_startuppercut", "Asset/Animations/Enemy/NightMare/StartUpperCut/", 330, 248, 3);
	Load("nightmare_uppercut", "Asset/Animations/Enemy/NightMare/UpperCut/", 254, 500, 2);
	Load("nightmare_stun", "Asset/Animations/Enemy/NightMare/Stun/", { {184,286}, {184,286}, {386,390}, {386,390}, {386,390}, {301,279}, {301,279} });
	Load("nightmare_fly", "Asset/Animations/Enemy/NightMare/Fly/", 113, 126, 3);
	Load("nightmare_turn", "Asset/Animations/Enemy/NightMare/Turn/", 130, 128, 2);
	Load("nightmare_die", "Asset/Animations/Enemy/NightMare/Die/", 189, 282, 3);
	Load("nightmare_scream", "Asset/Animations/Enemy/NightMare/Scream/", 308, 262, 3);
	Load("nightmare_fingerstretch", "Asset/Animations/Enemy/NightMare/FingerStretch/", 90, 97, 6);
	Load("nightmare_fingerclick", "Asset/Animations/Enemy/NightMare/FingerClick/", { {90,97}, {90,97}, {111,86}, {111,86}, {111,86} });
	Load("nightmare_stand", "Asset/Animations/Enemy/NightMare/Stand/", 163, 320, 12);

	//Water
	Load("water_fountain", "Asset/Animations/WaterFalls/", 31, 188, 10);
	Load("water_top", "Asset/Animations/WaterTop/", 343, 48, 9);
	Load("rain_land", "Asset/Animations/RainLand/", 120, 20, 6);

	//Others
	Load("pointer", "Asset/Animations/Pointer/", 43, 31, 11);
	Load("menuhit", "Asset/Animations/MenuHit/", 350, 51, 6);
	Load("dart", "Asset/Animations/Dart/", 71, 74, 5);
	Load("spike_ready", "Asset/Animations/Spike/Ready/", 58, 136, 3);
	Load("spike_start", "Asset/Animations/Spike/Start/", 64, 352, 4);
	Load("spike_idle", "Asset/Animations/Spike/Idle/", 57, 685, 2);
	Load("spike_end", "Asset/Animations/Spike/End/", 73, 524, 5);
	Load("flame_torch", "Asset/Animations/grimm_particle_flame.png", 738, 82, 9, 1, 9);
	Load("glow_torch", "Asset/Animations/TorchGlow/", 240, 240, 3);
	Load("firebat_idle", "Asset/Animations/FireBat/Idle/", 163, 136, 5);
	Load("firebat_destroy", "Asset/Animations/FireBat/Destroy/", 163, 136, 3);

	/** 粒子 **/
	Load("rain_bg", "Asset/Particles/rain_particle.png", 12, 1200, 3, 3, 1);
	Load("rain_bg_", "Asset/Particles/rain_particle_.png", 16, 1000, 3, 3, 1);
	Load("particle_heal", "Asset/Particles/heal_particle.png", 30, 80, 3, 3, 1);
	Load("particle_rock", "Asset/Particles/particles_barrel.png", 43, 376, 6, 6, 1);
	Load("menu_radiant_bottom", "Asset/Particles/gg_menu_radiant_0000_1.png", 15, 15, 1, 1, 1);
	Load("menu_radiant_top", "Asset/Particles/gg_menu_radiant_0000_2.png", 15, 15, 1, 1, 1);
	Load("menu_smoke", "Asset/Particles/wispy_smoke_particle_abyss.png", 306, 1536, 5, 5, 1);
	Load("menu_ss_particle", "Asset/Particles/ember_particle.png", 90, 90, 1, 1, 1);
	Load("particle_silk", "Asset/Particles/web_particle.png", 46, 183, 4, 4, 1);
	Load("particle_smoke", "Asset/Particles/hot_spring_smoke.png", 98, 87, 1, 1, 1);
	Load("particle_fragment", "Asset/Particles/rock_particle.png", 33, 88, 4, 4, 1);
	Load("particle_quake", "Asset/Particles/quake_particle.png", 36, 144, 4, 4, 1);
	Load("particle_spatter", "Asset/Particles/spatter_white.png", 31, 31, 1, 1, 1);
	Load("particle_fire", "Asset/Particles/ember_particle_round.png", 28, 27, 1, 1, 1);
	Load("particle_grimm_smoke", "Asset/Particles/grimm_smoke.png", 204, 1024, 5, 5, 1);
	Load("particle_flame_i", "Asset/Particles/wispy_flame_particle_i.png", 102, 510, 5, 5, 1);
	Load("particle_flame_o", "Asset/Particles/wispy_flame_particle_o.png", 204, 1020, 5, 5, 1);

	/** 图像 **/

	//Environment
	Load("menu_0", "Asset/Images/Menu/Voidheart_menu_BG.jpg", WIN_WIDTH + 50, WIN_HEIGHT + 75);
	Load("menu_1", "Asset/Images/Menu/title-bg.png", float(WIN_WIDTH) * 7 / 6, float(WIN_HEIGHT));
	Load("menu_2", "Asset/Images/Menu/title-f.png", 475, 750);

	Load("tearcity_0", "Asset/Images/BackGround/TearCity/0.png", 2100, 1350);
	Load("tearcity_1", "Asset/Images/BackGround/TearCity/ruins_bg_building_mid.png", 1500, 1750);
	Load("tearcity_2", "Asset/Images/BackGround/TearCity/ruins_mid_buildings.png", 600, 2000);
	Load("tearcity_3", "Asset/Images/BackGround/TearCity/ruins_bg_bridge_near_a.png", 1000, 512);
	Load("tearcity_4", "Asset/Images/BackGround/TearCity/ruin_roof_bg.png", 1250, 1000);
	Load("tearcity_5", "Asset/Images/BackGround/TearCity/BG_far_0002_a.png", 720, 1280);

	Load("tearcity_mid_0", "Asset/Images/BackGround/TearCity/ruins_bg_building_mid.png", 193, 256);

	Load("tearcity_environment_0", "Asset/Images/Environment/TearCity/0.png", 566, 672);
	Load("tearcity_environment_1", "Asset/Images/Environment/TearCity/1.png", 696, 128);
	Load("tearcity_environment_2", "Asset/Images/Environment/TearCity/4.png", 396, 512);
	Load("tearcity_environment_3", "Asset/Images/Environment/TearCity/5.png", 500, 750);
	Load("tearcity_water", "Asset/Images/Environment/TearCity/Lake_water.png", 3500, 160);

	Load("tearcity_environment_4", "Asset/Images/Environment/TearCity/_0025_city_break.png", 114, 410);
	Load("tearcity_environment_5", "Asset/Images/Environment/TearCity/_0029_city_break.png", 61, 308);
	Load("tearcity_environment_6", "Asset/Images/Environment/TearCity/ruin_sign_01.png", 72, 218);
	Load("tearcity_environment_7", "Asset/Images/Environment/TearCity/corpse_set_01_0005_nail4-copy.png", 23, 105);
	Load("tearcity_environment_8", "Asset/Images/Environment/TearCity/corpse_set_01_0008_nail1-copy_large.png", 100, 330);
	Load("tearcity_environment_9", "Asset/Images/Environment/TearCity/corpse_set_01_0006_nail2.png", 47, 198);
	Load("tearcity_environment_10", "Asset/Images/Environment/TearCity/_0017_city_break.png", 39, 333);
	Load("tearcity_environment_11", "Asset/Images/Environment/TearCity/_0032_city_break.png", 75, 270);
	Load("tearcity_environment_12", "Asset/Images/Environment/TearCity/ruin_lamp_01.png", 206, 310);
	Load("tearcity_environment_13", "Asset/Images/Environment/TearCity/_0022_city_break.png", 130, 300);
	Load("tearcity_environment_14", "Asset/Images/Environment/TearCity/_0030_city_break.png", 74, 315);
	Load("tearcity_environment_15", "Asset/Images/Environment/TearCity/ruin_small_corner_piece.png", 170, 250);
	Load("tearcity_environment_16", "Asset/Images/Environment/TearCity/_0016_city_break.png", 61, 324);
	Load("tearcity_environment_17", "Asset/Images/Environment/TearCity/_0019_city_break.png", 63, 329);
	Load("tearcity_environment_18", "Asset/Images/Environment/TearCity/_0024_city_break.png", 63, 329);
	Load("tearcity_environment_19", "Asset/Images/Environment/TearCity/_0021_city_break.png", 114, 410);
	Load("tearcity_fence", "Asset/Images/Environment/TearCity/ruin_layered_0013_f.png", 225, 180);
	Load("tearcity_roof", "Asset/Images/Environment/TearCity/ruind_bridge_roof_02_r.png", 608, 624);
	Load("tearcity_window", "Asset/Images/Environment/TearCity/ruind_window_03.png", 500, 400);
	Load("tearcity_doorbg", "Asset/Images/Environment/TearCity/ruin_add_doorbg_straight.png", 116, 236);

	Load("tearcity_plat_0", "Asset/Images/PlatForm/TearCity/ruin_plat_float_01.png", 163, 118);
	Load("tearcity_plat_1", "Asset/Images/PlatForm/TearCity/ruin_plat_float_02.png", 114, 83);
	Load("tearcity_plat_2", "Asset/Images/PlatForm/TearCity/ruin_plat_float_03.png", 111, 100);
	Load("tearcity_plat_3", "Asset/Images/PlatForm/TearCity/ruin_plat_float_04.png", 163, 115);
	Load("tearcity_plat_4", "Asset/Images/PlatForm/TearCity/ruin_plat_float_05_wide.png", 387, 137);
	Load("tearcity_floor", "Asset/Images/PlatForm/TearCity/Floor_1.png", 1177, 220);
	Load("tearcity_glassfloor", "Asset/Images/PlatForm/TearCity/mage_lord_room_0002_floor.png", 1580, 173);
	Load("tearcity_additions", "Asset/Images/PlatForm/TearCity/ruins_additions_0036_a.png", 371, 80);

	Load("chest", "Asset/Images/Chest - atlas0 #087042(1).png", 125, 144);
	Load("chest_", "Asset/Images/Chest - atlas0 #087042(2).png", 125, 118);
	Load("chair", "Asset/Images/chair.png", 184, 111);
	Load("beam", "Asset/Images/GG_beam_straight.png", 225, 300);

	Load("ruinhouse_0", "Asset/Images/BackGround/RuinHouse/ruin_house_BG.jpg", 1000, 580);
	Load("ruinhouse_1", "Asset/Images/BackGround/RuinHouse/room_ruin_house_FG.png", 1500, 820);

	Load("grimmtent_bg", "Asset/Images/Environment/GrimmTents/sleep_grimm_bg.png", 320, 526);
	Load("grimmtent_extrance", "Asset/Images/Environment/GrimmTents/Main_tent_entrance.png", 248, 462);
	Load("grimmtent_extrance_front", "Asset/Images/Environment/GrimmTents/Main_tent_entrance_front.png", 152, 410);
	Load("grimmtent_extrance_light", "Asset/Images/Environment/GrimmTents/SpriteAtlasTexture-Divine-1024x2048-fmt12.png", 240, 244);
	Load("grimmtent_ground_0", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0010_2.png", 892, 79);
	Load("grimmtent_ground_1", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0009_3.png", 880, 92);
	Load("grimmtent_ground_2", "Asset/Images/Environment/GrimmTents/grimm_curtain.png", 554, 112);
	Load("grimmtent_ground_3", "Asset/Images/Environment/GrimmTents/attic_0005_6.png", 1008, 224);
	Load("grimmtent_short_pole", "Asset/Images/Environment/GrimmTents/main_tent_short_pole.png", 142, 168);
	Load("grimmtent_wallpaper", "Asset/Images/Environment/GrimmTents/grimm_wallpaper.png", 418, 320);
	Load("grimmtent_curtain", "Asset/Images/Environment/GrimmTents/grimm_curtain_02.png", 228, 408);
	Load("grimmtent_curtain_", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0011_1.png", 476, 642);
	Load("grimmtent_ceiling", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0008_4.png", 409, 143);
	Load("grimmtent_ceiling_0", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0007_5.png", 577, 196);
	Load("grimmtent_ceiling_1", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0006_6.png", 158, 80);
	Load("grimmtent_pole_standard", "Asset/Images/Environment/GrimmTents/grimm_pole_standard.png", 125, 600);
	Load("grimmtent_rag", "Asset/Images/Environment/GrimmTents/grimm_rag_main_tent.png", 175, 325);
	Load("grimmtent_torch", "Asset/Images/Environment/GrimmTents/Divine_Room_0002_4.png", 103, 328);
	Load("grimmtent_stand", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0004_8.png", 499, 301);
	Load("grimmtent_pole", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0005_7.png", 65, 458);
	Load("grimmtent_flag", "Asset/Images/Environment/GrimmTents/Grimm_Main_tent_0000_12.png", 100, 525);
	Load("grimmtent_beam", "Asset/Images/Environment/GrimmTents/grimm_beam.png", 300, 800);

	
	//Others
	Load("player_light", "Asset/Images/grimm_fader.png", 600, 500);
	Load("death_light", "Asset/Images/dead_light_fader.png", 600, 600);
	Load("1geo", "Asset/Images/1Geo.png", 32, 31);
	Load("5geo", "Asset/Images/5Geo.png", 33, 44);
	Load("25geo", "Asset/Images/25Geo.png", 40, 44);
	Load("dart", "Asset/Images/dart.png", 73, 74);
	Load("needle", "Asset/Images/needle.png", 211, 16);
	Load("soul_orb", "Asset/Images/soul_orb.png", 100, 100);
	Load("roar_effect", "Asset/Images/roar_effect.png", 250, 250);
	Load("roar_effect_white", "Asset/Images/Roar_lines_white.png", 294, 283);
	Load("pointer_sit", "Asset/Images/prompt_idle0000.png", 170, 141);
	Load("wave1", "Asset/Images/Wave/1.png", 70, 90);
	Load("wave2", "Asset/Images/Wave/2.png", 80, 170);
	Load("wave3", "Asset/Images/Wave/3.png", 90, 200);
	Load("wave4", "Asset/Images/Wave/4.png", 70, 130);
	Load("wave5", "Asset/Images/Wave/5.png", 60, 120);
	Load("wave6", "Asset/Images/Wave/6.png", 50, 60);
	Load("gate", "Asset/Images/mines_metal_grate_02.png", 57, 269);

	//UI
	Load("inventory_item", "Asset/Images/UI/ins_item.png", 60, 60);
	Load("inventory_item_", "Asset/Images/UI/ins_item_bk.png", 60, 60);
	Load("inventory_coin", "Asset/Images/UI/InventoryCoin.png", 47, 48);
	Load("black", "Asset/Images/UI/mask_black.png", WIN_WIDTH + 12, WIN_HEIGHT + 8);
	Load("white", "Asset/Images/UI/mask_white.png", WIN_WIDTH, WIN_HEIGHT);
	Load("red", "Asset/Images/UI/mask_red.png", WIN_WIDTH, WIN_HEIGHT);
	Load("bar_white", "Asset/Images/UI/bar_white.png", 198, 3);
	Load("low_health", "Asset/Images/UI/vignette_low_health.png", WIN_WIDTH, WIN_HEIGHT);
	Load("inventory_ins1", "Asset/Images/UI/ins_1.png", 72, 72);
	Load("inventory_ins2", "Asset/Images/UI/ins_2.png", 173, 49);
	Load("inventory_ins3", "Asset/Images/UI/ins_3.png", 104, 44);
	Load("menu_title", "Asset/Images/Menu/title.png", 782, 404);
	Load("menu_icon", "Asset/Images/Menu/team_cherry_logo_main_menu.png", 250, 203);
	Load("menu_logo", "Asset/Images/Menu/Hidden_Dreams_Logo.png", 141, 38);
	Load("menu_slider", "Asset/Images/Menu/MenuSliderHandle.png", 110, 50);


	/*************
	 * 媒体资源加载
	 *************/
	//Music
	Load("tearcity_i", "Asset/Sounds/tearcity_i.mp3");
	Load("tearcity_o", "Asset/Sounds/tearcity_o.mp3");
	Load("tearcity_boss", "Asset/Sounds/tearcity_boss.mp3");
	Load("tearcity_boss_", "Asset/Sounds/tearcity_boss_.mp3");
	Load("menu", "Asset/Sounds/menu.mp3");
	Load("menu_", "Asset/Sounds/menu_.mp3");
	Load("grimmtent", "Asset/Sounds/grimmtent.mp3");
	Load("nightmare", "Asset/Sounds/nightmare.mp3");
	Load("nightmare_", "Asset/Sounds/nightmare_.mp3");
	Load("bossdefeat", "Asset/Sounds/bossdefeat.mp3");
	Load("bossdefeat_", "Asset/Sounds/bossdefeat_.mp3");
	Load("accordion", "Asset/Sounds/accordion.mp3");

	//Player
	Load("sound_waterwalk", "Asset/Sounds/Player/waterwalk.mp3");
	Load("sound_swim", "Asset/Sounds/Player/swim.mp3");
	Load("sound_rush", "Asset/Sounds/Player/run.mp3");
	Load("sound_attack", "Asset/Sounds/Player/attack.mp3");
	Load("sound_dash", "Asset/Sounds/Player/dash.mp3");
	Load("sound_damage_0", "Asset/Sounds/Player/sound_damage_0.mp3");
	Load("sound_damage_1", "Asset/Sounds/Player/sound_damage_1.mp3");
	Load("sound_damage_2", "Asset/Sounds/Player/sound_damage_2.mp3");
	Load("sound_hurt", "Asset/Sounds/Player/hurt.mp3");
	Load("sound_throw", "Asset/Sounds/Player/sound_throw.mp3");
	Load("sound_claw", "Asset/Sounds/Player/claw.mp3");
	Load("sound_darthit", "Asset/Sounds/Player/sound_darthit.mp3");
	Load("sound_counter", "Asset/Sounds/Player/sound_counter.mp3");
	Load("voice_throw", "Asset/Sounds/Player/voice_throw.mp3");
	Load("sound_jump", "Asset/Sounds/Player/sound_jump.mp3");
	Load("sound_walljump", "Asset/Sounds/Player/walljump.mp3");
	Load("voice_jump_0", "Asset/Sounds/Player/voice_jump_0.mp3");
	Load("voice_jump_1", "Asset/Sounds/Player/voice_jump_1.mp3");
	Load("voice_jump_2", "Asset/Sounds/Player/voice_jump_2.mp3");
	Load("sound_rest", "Asset/Sounds/Player/bench_rest.mp3");
	Load("sound_cure", "Asset/Sounds/Player/sound_cure.mp3");
	Load("voice_cure", "Asset/Sounds/Player/voice_cure.mp3");
	Load("sound_closeskill", "Asset/Sounds/Player/sound_closeskill.mp3");
	Load("voice_closeskill_0", "Asset/Sounds/Player/voice_closeskill_0.mp3");
	Load("voice_closeskill_1", "Asset/Sounds/Player/voice_closeskill_1.mp3");
	Load("sound_remoteskill", "Asset/Sounds/Player/sound_remoteskill.mp3");
	Load("voice_remoteskill_0", "Asset/Sounds/Player/voice_remoteskill_0.mp3");
	Load("voice_remoteskill_1", "Asset/Sounds/Player/voice_remoteskill_1.mp3");
	Load("sound_evade", "Asset/Sounds/Player/evade.mp3");
	Load("sound_evade_0", "Asset/Sounds/Player/evade_0.mp3");
	Load("sound_evade_1", "Asset/Sounds/Player/evade_1.mp3");
	Load("sound_stun", "Asset/Sounds/Player/stun.mp3");
	Load("sound_stun_", "Asset/Sounds/Player/stun_.mp3");
	Load("sound_hardland", "Asset/Sounds/Player/hardland.mp3");
	Load("sound_softland", "Asset/Sounds/Player/softland.mp3");
	Load("sound_land", "Asset/Sounds/Player/land.mp3");
	Load("sound_waterland", "Asset/Sounds/Player/waterland.mp3");
	Load("sound_blockhit", "Asset/Sounds/Player/blockhit.mp3");
	Load("voice_die", "Asset/Sounds/Player/voice_die.mp3");
	Load("sound_die", "Asset/Sounds/Player/sound_die.mp3");
	Load("sound_swordhit", "Asset/Sounds/Player/sound_swordhit.mp3");
	Load("sound_defend", "Asset/Sounds/Player/sound_defend.mp3");
	Load("sound_leave", "Asset/Sounds/Player/sound_leave.mp3");
	Load("sound_hardattack", "Asset/Sounds/Player/hardattack.mp3");

	//Geo
	Load("sound_geo_0", "Asset/Sounds/Geo/geo_collect_0.mp3");
	Load("sound_geo_1", "Asset/Sounds/Geo/geo_collect_1.mp3");
	Load("sound_geo_rockhit_0", "Asset/Sounds/Geo/geo_rock_hit_0.mp3");
	Load("sound_geo_rockhit_1", "Asset/Sounds/Geo/geo_rock_hit_1.mp3");
	Load("sound_geo_rockhit_2", "Asset/Sounds/Geo/geo_rock_hit_2.mp3");
	Load("sound_geo_hitground_0", "Asset/Sounds/Geo/geo_hit_ground_0.mp3");
	Load("sound_geo_hitground_1", "Asset/Sounds/Geo/geo_hit_ground_1.mp3");

	//Gate
	Load("sound_gate_open", "Asset/Sounds/Gate/gate_open.mp3");
	Load("sound_gate_close", "Asset/Sounds/Gate/gate_slam.mp3");

	//UI
	Load("sound_button_confirm", "Asset/Sounds/UI/ui_button_confirm.mp3");
	Load("sound_change_selection", "Asset/Sounds/UI/ui_change_selection.mp3");
	Load("sound_save", "Asset/Sounds/UI/ui_save.mp3");
	

	//Bug
	Load("sound_bug_appear", "Asset/Sounds/Bug/appear.mp3");
	Load("sound_bug_appear_", "Asset/Sounds/Bug/appear_.mp3");
	Load("sound_bug_die", "Asset/Sounds/Bug/die.mp3");

	//Fly
	Load("sound_fly_chase_0", "Asset/Sounds/Fly/chase_0.mp3");
	Load("sound_fly_chase_1", "Asset/Sounds/Fly/chase_1.mp3");
	Load("sound_fly_die", "Asset/Sounds/Fly/die.mp3");

	//SoulMaster
	Load("sound_soulmaster_roar", "Asset/Sounds/SoulMaster/roar.mp3");
	Load("sound_soulmaster_cast_0", "Asset/Sounds/SoulMaster/cast_0.mp3");
	Load("sound_soulmaster_cast_1", "Asset/Sounds/SoulMaster/cast_1.mp3");
	Load("sound_soulmaster_cast_2", "Asset/Sounds/SoulMaster/cast_2.mp3");
	Load("sound_soulmaster_cast_3", "Asset/Sounds/SoulMaster/cast_3.mp3");
	Load("startstun", "Asset/Sounds/SoulMaster/startstun.mp3");
	Load("stun", "Asset/Sounds/SoulMaster/stun.mp3");
	Load("startdeath", "Asset/Sounds/SoulMaster/startdeath.mp3");
	Load("death", "Asset/Sounds/SoulMaster/death.mp3");
	Load("sound_soulmaster_angry_0", "Asset/Sounds/SoulMaster/angry_0.mp3");
	Load("sound_soulmaster_angry_1", "Asset/Sounds/SoulMaster/angry_1.mp3");
	Load("sound_soulmaster_angry_2", "Asset/Sounds/SoulMaster/angry_2.mp3");
	Load("sound_soulmaster_angry_3", "Asset/Sounds/SoulMaster/angry_3.mp3");
	Load("sound_soulmaster_angry_4", "Asset/Sounds/SoulMaster/angry_4.mp3");
	Load("sound_soulmaster_angry_5", "Asset/Sounds/SoulMaster/angry_5.mp3");
	Load("sound_soulmaster_angry_6", "Asset/Sounds/SoulMaster/angry_6.mp3");
	Load("sound_soulmaster_teleport", "Asset/Sounds/SoulMaster/teleport.mp3");
	Load("sound_soulmaster_ground", "Asset/Sounds/SoulMaster/ground.mp3");
	Load("sound_soulmaster_slash", "Asset/Sounds/SoulMaster/slash.mp3");

	Load("sound_rumble", "Asset/Sounds/SoulMaster/rumble.mp3");
	Load("sound_explode", "Asset/Sounds/SoulMaster/explode.mp3");

	//NightMare
	Load("sound_nightmare_teleportin", "Asset/Sounds/NightMare/teleport_in.mp3");
	Load("sound_nightmare_teleportout", "Asset/Sounds/NightMare/teleport_out.mp3");
	Load("sound_spike_grounded", "Asset/Sounds/NightMare/spike_grounded.mp3");
	Load("sound_spike_shootup", "Asset/Sounds/NightMare/spike_shoot_up.mp3");
	Load("sound_spike_shrivelback", "Asset/Sounds/NightMare/spike_shrivel_back.mp3");
	Load("sound_nightmare_fireballs", "Asset/Sounds/NightMare/balloon_fireballs_loop.mp3");
	Load("sound_nightmare_cast_1", "Asset/Sounds/NightMare/cast_1.mp3");
	Load("sound_nightmare_cast_2", "Asset/Sounds/NightMare/cast_2.mp3");
	Load("sound_nightmare_airdash", "Asset/Sounds/NightMare/airdash.mp3");
	Load("sound_nightmare_dash", "Asset/Sounds/NightMare/dash.mp3");
	Load("sound_nightmare_uppercut", "Asset/Sounds/NightMare/uppercut.mp3");
	Load("sound_fireball_cast", "Asset/Sounds/NightMare/fireball_cast.mp3");
	Load("sound_skyflame", "Asset/Sounds/NightMare/sky_flame.mp3");
	Load("sound_sword_0", "Asset/Sounds/NightMare/sword_0.mp3");
	Load("sound_sword_1", "Asset/Sounds/NightMare/sword_1.mp3");
	Load("sound_nightmare_explode", "Asset/Sounds/NightMare/explode_into_bats.mp3");
	Load("sound_nightmare_circling", "Asset/Sounds/NightMare/bats_circling.mp3");
	Load("sound_nightmare_appear", "Asset/Sounds/NightMare/appear.mp3");
	Load("sound_nightmare_reform", "Asset/Sounds/NightMare/bats_reform.mp3");
	Load("sound_nightmare_defeat", "Asset/Sounds/NightMare/defeat.mp3");
	Load("sound_nightmare_scream", "Asset/Sounds/NightMare/scream.mp3");
	Load("sound_nightmare_deflate", "Asset/Sounds/NightMare/balloon_deflate.mp3");
	Load("sound_nightmare_click", "Asset/Sounds/NightMare/click.mp3");


	Load("sound_boss_gushing", "Asset/Sounds/boss_gushing.mp3");
	Load("sound_boss_stun", "Asset/Sounds/boss_stun.mp3");
	Load("sound_boss_finalhit", "Asset/Sounds/boss_finalhit.mp3");

	/*************
	 * 视频资源加载
	 *************/
	Load("video_start", L"Asset/Videos/start.avi");

	/*************
	 * 字体资源加载
	 *************/
	LoadText("Asset/TrajanPro.ttf");

	/*************
	 * 鼠标样式资源加载
	 *************/
	LoadCustomCursor("Asset/cursor.cur");
}

//*********                             **********
//************************************************




IMAGE* ResourceManager::Fetch(std::string name)
{
	return imagePool[name];
}

ArtyEngine::FAnimationResource ResourceManager::FetchAnimation(std::string name)
{
	return animationPool[name];
}

std::wstring ResourceManager::FetchVideo(std::string name)
{
	return videoPool[name];
}


void ResourceManager::Load(std::string name, std::string path, int wid, int hei)
{
	IMAGE* img = new IMAGE;
	loadimage(img, LPCTSTR(path.c_str()),wid,hei);
	imagePool.insert({name, img});
}

void ResourceManager::Load(std::string name, std::string path, int wid, int hei, int num)
{
	IMAGE**images = new IMAGE*[num];
	for (int i = 0; i < num; i++)
	{
		images[i] = new IMAGE;
		loadimage(images[i], LPCTSTR((path + "/" + std::to_string(i) + ".png").c_str()), wid, hei);
	}
	animationPool.insert({ name, {images,num } });
}

void ResourceManager::Load(std::string name, std::string path, const std::vector<FIntVector2>& arr)
{
	int num = int(arr.size());
	IMAGE** images = new IMAGE * [num];
	for (int i = 0; i < num; i++)
	{
		images[i] = new IMAGE;
		loadimage(images[i], LPCTSTR((path + "/" + std::to_string(i) + ".png").c_str()), arr[i].x, arr[i].y);
	}
	animationPool.insert({ name, {images,int(arr.size())} });
}


void ResourceManager::Load(std::string name, std::string path, int wid, int hei, int num, int row, int col)
{
	IMAGE** images = new IMAGE * [num];
	IMAGE img;
	loadimage(&img, LPCTSTR(path.c_str()), wid, hei);
	DWORD* src = GetImageBuffer(&img);

	int width = wid / col;
	int height = hei / row;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			int x = j * width;
			int y = i * height;
			int index = i * col + j;
			if (index >= num)break;
			images[index] = new IMAGE(width, height);
			DWORD* dst = GetImageBuffer(images[index]);

			for (int x1 = 0; x1 < width; ++x1)
			{
				for (int y1 = 0; y1 < height; ++y1)
				{
					dst[x1 + y1 * width] = src[x + x1 + (y + y1) * wid];
				}
			}
		}
	}
	animationPool.insert({ name, {images,num } });
}


void ResourceManager::Load(std::string name, std::string path)
{
	std::string file = std::string("open ") + path + std::string(" alias ") + name;
	mciSendString(file.c_str(), NULL, 0, NULL);
}

void ResourceManager::Load(std::string name, std::wstring pszFileName)
{
	videoPool[name] = pszFileName;
}

void ResourceManager::LoadText(std::string path)
{
	AddFontResourceEx(path.c_str(), FR_PRIVATE, NULL);
}

void ResourceManager::LoadCustomCursor(std::string path)
{
	// 加载光标文件
	HCURSOR hcur = (HCURSOR)LoadImage(NULL, path.c_str(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
	if (hcur == NULL)
	{
		return;
	}

	ICONINFO iconInfo;
	if (GetIconInfo(hcur, &iconInfo))
	{
		// 获取光标图像的宽度和高度
		BITMAP bm;
		GetObject(iconInfo.hbmColor, sizeof(BITMAP), &bm);
		int cursorWidth = bm.bmWidth;
		int cursorHeight = bm.bmHeight;

		// 设置新的热点位置
		ICONINFO newIconInfo = iconInfo;
		newIconInfo.xHotspot = cursorWidth / 2; 
		newIconInfo.yHotspot = cursorHeight / 2; 

		// 创建新的光标
		HCURSOR newHcur = CreateIconIndirect(&newIconInfo);
		if (newHcur == NULL)
		{
			DeleteObject(iconInfo.hbmColor);
			DeleteObject(iconInfo.hbmMask);
			return;
		}

		DeleteObject(iconInfo.hbmColor);
		DeleteObject(iconInfo.hbmMask);

		// 设置新的光标
		SetClassLongPtr(GetHWnd(), GCLP_HCURSOR, (LONG_PTR)newHcur);
	}
	else
	{
		DeleteObject(hcur);
	}
}
