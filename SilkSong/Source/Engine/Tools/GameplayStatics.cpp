#include "GameplayStatics.h"
#include "Objects/Controller.h"
#include "LevelManager.h"
#include "Components/Camera.h"
#include "Tools/Timer.h"
#include "Tools/VisualInterface.h"
#include "UI/UserInterface.h"



extern bool bFrameLimitOn = false;

extern int MaxFrame = 100;

GameInstance* GameplayStatics::GetGameInstance()
{
    if(!mainWorld.gameInstance)mainWorld.gameInstance = new GameInstance;
    return mainWorld.gameInstance;
}

Controller* GameplayStatics::GetController()
{
    Controller* pController = mainWorld.currentLevel->mainController;
    if (!pController)
    {
        pController = mainWorld.currentLevel->mainController = CreateObject<Controller>();
    }
    return pController;
}

void GameplayStatics::OpenLevel(std::string levelName)
{
    mainWorld.levelManager->SetDefaultLevel(levelName);
}

void GameplayStatics::QuitGame()
{
    std::exit(0);
}

void GameplayStatics::PlayCameraShake(int intensity, int decay)
{
    mainWorld.mainCamera->ShakeCamera(intensity,decay);
}

double GameplayStatics::GetTimeSeconds()
{
    return mainWorld.OverallClock->GetDelay();
}

void GameplayStatics::SetGaussianFilterOn(bool enable, int level)
{
    ImageToolkit::bIsGaussianFilterOn = enable;
    ImageToolkit::GaussianFilterLevel = level;
}

void GameplayStatics::Pause(float delay)
{
    Sleep(uint32(1000*delay));
    mainWorld.deltaTime -= delay;
}

void GameplayStatics::PlaySound2D(std::string name)
{
    mciSendString((std::string("seek ") + name + std::string(" to start")).c_str(), 0, 0, 0);
    mciSendString((std::string("play ") + name).c_str(), 0, 0, 0);
}

void GameplayStatics::DontDestroyOnLoad(Actor* obj)
{
    mainWorld.OverallGameActors_to_add.push_back(obj);
}

void GameplayStatics::DontDestroyOnLoad(UserInterface* obj)
{
    mainWorld.OverallGameUIs_to_add.push_back(obj);
}

