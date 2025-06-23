#include "Level.h"
#include "Objects/Controller.h"


Level::Level()
{
    callback = [this]() {return mainController = GameplayStatics::CreateObject<Controller>(); };
}

void Level::BeginPlay()
{
    for (auto& obj : mainWorld.OverallGameActors)
    {
        if (Controller* controller = Cast<Controller>(obj))
        {
            mainController = controller; return;
        };
    }
	callback();
}