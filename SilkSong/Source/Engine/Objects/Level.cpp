#include "Level.h"
#include "Objects/Controller.h"


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