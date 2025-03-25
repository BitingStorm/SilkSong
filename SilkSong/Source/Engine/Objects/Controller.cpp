#include "Controller.h"
#include "Components/Camera.h"
#include "Components/Collider.h"
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
	return (InputComponent::GetMousePosition() - FVector2D(WIN_WIDTH, WIN_HEIGHT) * 0.5f)
		* mainWorld.mainCamera->GetVirtualSpringArmLength() / 20.f + mainWorld.mainCamera->GetWorldPosition();
}

bool Controller::IsMouseClicked() const
{
	return InputComponent::IsMouseButtonPressed();
}

bool Controller::IsAnyKeyPressed() const
{
	return InputComponent::IsAnyKeyPressed();
}

bool Controller::IsKeyPressed(EKeyCode keycode) const
{
	return InputComponent::IsKeyPressed(keycode);
}

FHitResult Controller::GetHitResultUnderCursor() const
{
	FVector2D pos = GetCursorPosition();
	int x = FMath::Clamp(int(pos.x + 2000) / 400, 0, 9);
	int y = FMath::Clamp(int(pos.y) / 200, 0, 5);

	if (!mainWorld.ColliderZones[x][y].empty())
	{
		for (auto it = mainWorld.ColliderZones[x][y].rbegin(); it != mainWorld.ColliderZones[x][y].rend(); ++it)
		{
			if ((*it)->IsMouseOver())return FHitResult(pos, {0,0}, (*it)->pOwner, *it);
		}
	}
	return FHitResult();
}

void Controller::EnableInput(bool enable)
{
	InputComponent::EnableInput(enable);
}


