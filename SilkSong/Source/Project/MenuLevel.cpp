#include "MenuLevel.h"
#include "GameplayStatics.h"
#include "MenuController.h"
#include "Bg.h"

void MenuLevel::BeginPlay()
{
	mainController = GameplayStatics::CreateObject<MenuController>();

	Bg* bg0 = GameplayStatics::CreateObject<Bg>({0,-25}); bg0->Init("menu_0");
}
