#include "MenuController.h"
#include "MenuUI.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"
#include "Components/ParticleSystem.h"
#include "OldTheme.h"
#include "NewTheme.h"



MenuController::MenuController()
{
	whiteRadiant = ConstructComponent<ParticleSystem>();
	whiteRadiant->SetLocalPosition({ 0,-200 });
	whiteRadiant->AttachTo(root);
	whiteRadiant->SetCapacity(20);
	whiteRadiant->SetInterval(0.2f);
	whiteRadiant->Load("menu_radiant_top");
	whiteRadiant->SetMaxSpeed(10);
	whiteRadiant->SetMinSpeed(0);
	whiteRadiant->SetPattern(EParticlePattern::Center);
	whiteRadiant->SetCenter({ 100,200 });
	whiteRadiant->SetLayer(1);
	whiteRadiant->SetLifeCycle(4.f);
	whiteRadiant->SetFadingOutTime(1.f);
	whiteRadiant->SetFadingInTime(1.f);
	whiteRadiant->SetGravity(0.1f);
	whiteRadiant->SetSizeRange(0.25, 0.5);

	newTheme = GameplayStatics::CreateObject<NewTheme>();
}

void MenuController::BeginPlay()
{
    Super::BeginPlay();
    GameplayStatics::CreateUI<MenuUI>()->AddToViewport();
	GameModeHelper::PlayBGMusic("menu_");
}

void MenuController::Update(float deltaTime)
{
    Super::Update(deltaTime);
}

void MenuController::ChangeTheme()
{
	if (oldTheme)
	{
		oldTheme->Destroy();
		oldTheme = nullptr;
		newTheme = GameplayStatics::CreateObject<NewTheme>();
		GameModeHelper::GetInstance()->GetAudioPlayer(0)->Stop("menu");
		GameModeHelper::PlayBGMusic("menu_");
	}
	else
	{
		newTheme->Destroy();
		newTheme = nullptr;
		oldTheme = GameplayStatics::CreateObject<OldTheme>();
		GameModeHelper::GetInstance()->GetAudioPlayer(0)->Stop("menu_");
		GameModeHelper::PlayBGMusic("menu");
	}
}