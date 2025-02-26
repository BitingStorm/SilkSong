#include "WidgetComponent.h"
#include "UI/UserInterface.h"
#include "GameplayStatics.h"


WidgetComponent::WidgetComponent()
{

}

void WidgetComponent::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (!bIsEnabled || !userInterface)return;
	
	userInterface->rootCanvas->SetRelativePosition(GameplayStatics::ProjectWorldToScreen(GetWorldPosition()));
}

void WidgetComponent::Activate()
{
	Super::Activate();

	if (userInterface)
	{
		userInterface->AddToViewport();
	}
}

void WidgetComponent::Deactivate()
{
	Super::Deactivate();

	if (userInterface)
	{
		userInterface->HideFromViewport();
	}
}

void WidgetComponent::SetUI(UserInterface* ui)
{
	userInterface = ui;
	if (!ui)return;
	userInterface->rootCanvas->SetRelativePosition(GameplayStatics::ProjectWorldToScreen(GetWorldPosition()));
}

void WidgetComponent::SetSize(FVector2D size)
{
	if (!userInterface)return;
	
	userInterface->rootCanvas->SetSize(size);
}

void WidgetComponent::RegisterDontDestroy()
{
	userInterface->RegisterDontDestroy();
}