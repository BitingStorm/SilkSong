#include "GrimmLevel.h"
#include "NightMare.h"
#include "GameModeHelper.h"
#include "LevelTransformer.h"
#include "PlatForm.h"
#include "Bg.h"
#include "Gate.h"
#include "Player.h"
#include "Components/Camera.h"
#include "Torch.h"
#include "Brumm.h"



GrimmLevel::GrimmLevel()
{
	GameMode* gameMode = GameModeHelper::GetInstance();
	gameMode->RefreshVolume();
	gameMode->GetAudioPlayer(0)->Pause("tearcity_i");
	gameMode->GetAudioPlayer(2)->Pause("tearcity_o");
	GameModeHelper::PlayBGMusic("grimmtent");
	GameplayStatics::CreateObject<LevelTransformer>({ 1475,900 })->Init("RuinHouse", { -550,900 }, FRect({ -100.f,250.f }, { 100.f,750.f }));
	GameplayStatics::CreateObject<Gate>({ -300,650 })->SetLocalScale({ -1.f,1.2f });

	GameplayStatics::CreateObject<PlatForm>({ 0,1030 })->Init("", { 3200,64 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 650,550 })->Init("", { 2000,100 }, {});
	GameplayStatics::CreateObject<PlatForm>({ 1525,800 })->Init("", { 150,400 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -1650,500 })->Init("", { 100,1000 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -900,100 })->Init("", { 1500,100 }, {});
	GameplayStatics::CreateObject<PlatForm>({ -300,300 })->Init("", { 100,400 }, {});

	GameplayStatics::CreateObject<Bg>({ 1350,800 })->Init("grimmtent_extrance");
	GameplayStatics::CreateObject<Bg>({ 1400,820 })->Init("grimmtent_extrance_front", 5, false, false);
	GameplayStatics::CreateObject<Bg>({ 1330,875 })->Init("grimmtent_extrance_light", 2, false, false);

	for (int i = 0; i < 4; i++)
	{
		GameplayStatics::CreateObject<Bg>({ 750.f * (1.5f - i) - 100.f,1000.f })->Init("grimmtent_ground_0", 2);
	}

	for (int i = 0; i < 5; i++)
	{
		GameplayStatics::CreateObject<Bg>({ 680.f * (2.f - i) - 100.f,1010.f })->Init("grimmtent_ground_1", 3);
	}

	GameplayStatics::CreateObject<Bg>({ -1000.f,990.f })->Init("grimmtent_ground_2", 5);
	GameplayStatics::CreateObject<Bg>({ 1500.f,975.f })->Init("grimmtent_ground_3", 5);
	GameplayStatics::CreateObject<Bg>({ 950.f,1010.f })->Init("grimmtent_ground_2", 4);
	GameplayStatics::CreateObject<Bg>({ -300.f,1025.f })->Init("grimmtent_ground_3", 5);
	GameplayStatics::CreateObject<Bg>({ 230.f,950.f })->Init("grimmtent_short_pole", 4);
	GameplayStatics::CreateObject<Bg>({ -335.f,410.f })->Init("grimmtent_curtain_", 4, false, false);

	for (int i = 0; i < 3; i++)
	{
		GameplayStatics::CreateObject<Bg>({ 550.f * (2.f - i),930.f })->Init("grimmtent_wallpaper", -3);
		GameplayStatics::CreateObject<Bg>({ 550.f * (2.f - i),675.f })->Init("grimmtent_ceiling", -2);
		GameplayStatics::CreateObject<Bg>({ 550.f * (2.f - i),615.f })->Init("grimmtent_ceiling_0", 3, false, false);
		GameplayStatics::CreateObject<Bg>({ 550.f * (1.5f - i),725.f })->Init("grimmtent_ceiling_1", 4, false, false);
		GameplayStatics::CreateObject<Bg>({ 550.f * (2.f - i) - 275.f,820.f })->Init("grimmtent_curtain");
		GameplayStatics::CreateObject<Bg>({ 500.f * (2.f - i),825.f })->Init("grimmtent_pole_standard", 5);
		Bg* rag = GameplayStatics::CreateObject<Bg>({ 500.f * (2.f - i) + FMath::RandInt(0, 250), 700.f });
		rag->Init("grimmtent_rag", 5);
		rag->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(1.f, 1.25f));
	}

	GameplayStatics::CreateObject<Torch>({ -700.f,325.f });
	GameplayStatics::CreateObject<Torch>({ -1500.f,350.f }, 0.f, { 1.1f,1.1f });
	GameplayStatics::CreateObject<Torch>({ -1000.f,300.f }, 0.f, { 0.8f,0.8f });

	GameplayStatics::CreateObject<Brumm>({ 500.f,900.f });

	if (Player* player = GameplayStatics::FindObjectOfClass<Player>())
	{
		this->player = player;
	}

	OnLevelDelete.Add([this]() {
		GameMode* gameMode = GameModeHelper::GetInstance();
		gameMode->GetAudioPlayer(0)->Stop("grimmtent");
		gameMode->GetAudioPlayer(0)->Stop("nightmare");
		gameMode->GetAudioPlayer(2)->Stop("nightmare_");
		gameMode->GetAudioPlayer(1)->Stop("sound_nightmare_fireballs");
		gameMode->GetAudioPlayer(1)->Stop("sound_nightmare_circling");
		});
}

void GrimmLevel::Update(float deltaTime)
{
	Level::Update(deltaTime);

	if (player && shakeTimer == 0 && player->GetWorldPosition().x < -1000.f)
	{
		GameplayStatics::FindObjectOfClass<Gate>()->Close();
		player->GetComponentByClass<Camera>()->SetRectFrame(FRect({ -1000.f,500.f }, { -750.f,680.f }));
		shakeTimer++;
		GameplayStatics::CreateObject<NightMare>({ -1250,850 }, 0.f, { -1.f,1.f });
		GameMode* gameMode = GameModeHelper::GetInstance();
		gameMode->GetAudioPlayer(0)->Stop("grimmtent");
	}
}
