#include "MenuLevel.h"
#include "GameplayStatics.h"
#include "MenuController.h"
#include "GameMode.h"

MenuLevel::MenuLevel()
{
	SetDefaultController<MenuController>();

	GameplayStatics::DontDestroyOnLoad(GameplayStatics::CreateObject<GameMode>());
}