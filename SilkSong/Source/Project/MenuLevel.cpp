#include "MenuLevel.h"
#include "GameplayStatics.h"
#include "MenuController.h"


void MenuLevel::BeginPlay()
{
	mainController = GameplayStatics::CreateObject<MenuController>();
}
