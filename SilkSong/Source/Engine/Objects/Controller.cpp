#include "Controller.h"
#include "Components/Camera.h"
#include "Core/World.h"



Controller::Controller()
{
	camera = ConstructComponent<Camera>();
	camera->AttachTo(root);
	camera->SetMainCamera();

	inputComponent = ConstructComponent<InputComponent>();
}

void Controller::BeginPlay()
{
	Super::BeginPlay();
	SetupInputComponent(inputComponent);
}

void Controller::PeekInfo()
{
	inputComponent->PeekInfo();
}

void Controller::PeekInfo_()
{
	inputComponent->PeekInfo_();
}

void Controller::MouseTick()
{
	inputComponent->MouseTick();
}

FVector2D Controller::GetCursorPosition() const
{
	return InputComponent::GetMousePosition() + mainWorld.mainCamera->GetWorldPosition() - FVector2D(WIN_WIDTH,WIN_HEIGHT)/2;
}

bool Controller::IsMouseClicked() const
{
	return InputComponent::IsMouseButtonPressed();
}

HitResult Controller::GetHitResultUnderCursor() const
{
	FVector2D pos = GetCursorPosition();
	int x = FMath::Clamp(int(pos.x + 2000) / 400, 0, 9);
	int y = FMath::Clamp(int(pos.y) / 200, 0, 5);

	if (!mainWorld.ColliderZones[x][y].empty())
	{
		for (auto it = mainWorld.ColliderZones[x][y].rbegin(); it != mainWorld.ColliderZones[x][y].rend(); ++it)
		{
			if ((*it)->IsMouseOver())return HitResult(pos, {0,0}, (*it)->pOwner, *it);
		}
	}
	return HitResult();
}

void Controller::EnableInput(bool enable)
{
	InputComponent::EnableInput(enable);
}


