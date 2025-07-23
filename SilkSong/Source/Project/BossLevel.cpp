#include "BossLevel.h"
#include "GameModeHelper.h"
#include "LevelTransformer.h"
#include "SoulMaster.h"
#include "PlatForm.h"
#include "Bg.h"
#include "RainProducer.h"
#include "Player.h"
#include "Gate.h"
#include "Components/Camera.h"



BossLevel::BossLevel()
{
	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->RefreshVolume();
	gameMode->GetAudioPlayer(0)->Pause("tearcity_i");
	gameMode->GetAudioPlayer(2)->Pause("tearcity_o");
	GameplayStatics::CreateObject<LevelTransformer>({ -1325,900 })->Init("TearCity", { 1700,875 }, FRect({ -375.f,-1000.f }, { 1225.f,725.f }));

	Bg* bg0 = GameplayStatics::CreateObject<Bg>(); bg0->Init("tearcity_0", -30, -30);
	Bg* bg1 = GameplayStatics::CreateObject<Bg>({ 0,1000 }); bg1->Init("tearcity_4", -10, -10);
	Bg* bg2 = GameplayStatics::CreateObject<Bg>({ -1200,600 }); bg2->Init("tearcity_2", -15, -15);
	Bg* bg3 = GameplayStatics::CreateObject<Bg>({ 1000,1300 }); bg3->Init("tearcity_5", -20, -20);
	GameplayStatics::CreateObject<Bg>({ -1000,500 })->Init("tearcity_roof", 2);
	GameplayStatics::CreateObject<Bg>({ -1075,800 })->Init("tearcity_window", -2, -2);
	GameplayStatics::CreateObject<Bg>({ -785,890 })->Init("tearcity_doorbg", -1, -1);
	GameplayStatics::CreateObject<Gate>({ -780,690 });

	GameplayStatics::CreateObject<PlatForm>({ 0,1075 })->Init("tearcity_glassfloor", { 1500,154 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1265,1030 })->Init("tearcity_additions", { 346,64 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -925,1030 })->Init("tearcity_additions", { 346,64 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 925,1030 })->Init("tearcity_additions", { 346,64 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1000,550 })->Init("", { 550,250 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1325,500 })->Init("", { 100,1400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1100,500 })->Init("", { 100,1400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -775,730 })->Init("", { 100,110 }, {});

	GameplayStatics::CreateObject<RainProducer>({ -200,-500 });


	for (int i = 0; i < 7; i++)
	{
		GameplayStatics::CreateObject<Bg>({ -750 + i * 250.f,950.f })->Init("tearcity_fence", -3, -3);
		Bg* beam = GameplayStatics::CreateObject<Bg>({ -765 + i * 235.f,925.f });
		beam->Init("beam", -1); beam->SetLocalScale(FVector2D(1.f, 1.9f - 0.3f * FMath::Abs(i - 3.f)));
	}

	if (Player* player = GameplayStatics::FindObjectOfClass<Player>())
	{
		this->player = player;
	}

	OnLevelDelete.AddLambda([]() {
		GameMode* gameMode = GameModeHelper::GetInstance();
		gameMode->GetAudioPlayer(0)->Stop("tearcity_boss");
		gameMode->GetAudioPlayer(0)->Stop("tearcity_boss_");
		});
}

void BossLevel::Update(float deltaTime)
{
	Level::Update(deltaTime);

	if (player && shakeTimer == 0 && player->GetWorldPosition().x > 0)
	{
		GameplayStatics::FindObjectOfClass<Gate>()->Close();
		shakeTimer++;
		GameModeHelper::PlayFXSound("sound_rumble");
		ShakeTimerHandle.Bind(0.3f, [this]() 
			{
			GameplayStatics::PlayCameraShake(4, 2); 
			if (++shakeTimer > 14) 
			{ 
				master = GameplayStatics::CreateObject<SoulMaster>({ 200,600 });
				GameModeHelper::PlayBGMusic("tearcity_boss"); 
				player->Scare(true, "scare");
				player->EnableInput(false);
				player->GetComponentByClass<Camera>()->SetRectFrame(FRect({ -400.f,-250.f }, { 475.f,690.f }));
				ShakeTimerHandle.Stop();
			}
			}, true);
	}
}
