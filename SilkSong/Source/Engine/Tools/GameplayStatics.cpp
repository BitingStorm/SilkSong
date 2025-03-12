#include "GameplayStatics.h"
#include "Objects/Level.h"
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

Actor* GameplayStatics::FindObjectOfName(std::string tagName)
{
    for (auto& obj : mainWorld.GameActors)
    {
        if (obj->GetName() == tagName)return obj;
    }
    for (auto& obj : mainWorld.OverallGameActors)
    {
        if (obj->GetName() == tagName)return obj;
    }
    return nullptr;
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

void GameplayStatics::Sleep(float delay)
{
    Sleep(uint32(1000 * delay));
    mainWorld.deltaTime -= delay;
}

void GameplayStatics::Pause(float delay)
{
    mainWorld.pauseDelay = delay;
    mainWorld.lastPauseTime = mainWorld.OverallClock->GetDelay();
}

void GameplayStatics::PlaySound2D(std::string name)
{
    mciSendString((std::string("seek ") + name + std::string(" to start")).c_str(), 0, 0, 0);
    mciSendString((std::string("play ") + name).c_str(), 0, 0, 0);
}

void GameplayStatics::DontDestroyOnLoad(Actor* obj)
{
    mainWorld.OverallGameActors_to_add.push_back(obj);
    obj->RegisterDontDestroy();
}

void GameplayStatics::DontDestroyOnLoad(UserInterface* obj)
{
    mainWorld.OverallGameUIs_to_add.push_back(obj);
    obj->RegisterDontDestroy();
}

FVector2D GameplayStatics::ProjectScreenToWorld(FVector2D pos)
{
    FVector2D position = (pos - FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f) * 0.05f
        * mainWorld.mainCamera->GetVirtualSpringArmLength() + mainWorld.mainCamera->GetVirtualPosition();
    return position;
}

FVector2D GameplayStatics::ProjectWorldToScreen(FVector2D pos)
{
    FVector2D position = (pos - mainWorld.mainCamera->GetVirtualPosition()) * 20.f
        / mainWorld.mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f;
    return position;
}