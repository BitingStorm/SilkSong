#include "MenuController.h"
#include "MenuUI.h"
#include "GameplayStatics.h"
#include "Components/AudioPlayer.h"
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
	whiteRadiant->SetPattern(ParticlePattern::Center);
	whiteRadiant->SetCenter({ 100,200 });
	whiteRadiant->SetLayer(1);
	whiteRadiant->SetLifeCycle(4.f);
	whiteRadiant->SetFadingOutTime(1.f);
	whiteRadiant->SetFadingInTime(1.f);
	whiteRadiant->SetGravity(0.1);
	whiteRadiant->SetSizeRange(0.25, 0.5);

	audio = ConstructComponent<AudioPlayer>();
	audio->AttachTo(root);
	audio->Play("menu", true);

	oldTheme = GameplayStatics::CreateObject<OldTheme>();
}

void MenuController::BeginPlay()
{
    Super::BeginPlay();
    GameplayStatics::CreateUI<MenuUI>()->AddToViewport();
	
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
		audio->Stop("menu");
		audio->Play("menu_", true);
	}
	else
	{
		newTheme->Destroy();
		newTheme = nullptr;
		oldTheme = GameplayStatics::CreateObject<OldTheme>();
		audio->Stop("menu_");
		audio->Play("menu", true);
	}
}
