#include "UserInterface.h"
#include "Core/World.h"



UserInterface::UserInterface()
{
	rootCanvas->SetSize({ WIN_WIDTH,WIN_HEIGHT }); 
	rootCanvas->SetRelativePosition({ WIN_WIDTH * 0.5,WIN_HEIGHT * 0.5 });
}

void UserInterface::Update(float deltaTime)
{
	for (auto& widget : widgets)
	{
		if (widget->GetUIPattern() != UIPattern::None)widget->Update();
	}
}

void UserInterface::ShowInfoBoxs()
{
	for (auto& widget : widgets) { widget->ShowInfoBox(); }
}

void UserInterface::DrawDebugRect()
{
	for (auto& widget : widgets) { widget->DrawDebugRect(); }
}

void UserInterface::AddToViewport()
{
	for (auto& widget : widgets)
	{
		widget->SetUIPattern(UIPattern::VisibleAndInteractive);
	}
}

void UserInterface::HideFromViewport()
{
	for (auto& widget : widgets)
	{
		widget->SetUIPattern(UIPattern::None);
	}
}

void UserInterface::RemoveFromViewport()
{
	mainWorld.GameUIs_to_delete.insert(this);
}