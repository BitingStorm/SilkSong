#include "UserInterface.h"
#include "Core/World.h"



UserInterface::UserInterface()
{
	rootCanvas->SetSize(FVector2D(WIN_WIDTH, WIN_HEIGHT));
	rootCanvas->SetRelativePosition({ WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f });
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
	for (auto& widget : widgets) { if (widget->GetUIPattern() != UIPattern::None)widget->DrawDebugRect(); }
}

void UserInterface::AddToViewport()
{
	OnAddedToViewport.BroadCast();
	for (auto& widget : widgets)
	{
		widget->SetUIPattern(UIPattern::VisibleAndInteractive);
	}
	for (auto& ui : userInterfaces)
	{
		ui->AddToViewport();
	}
}

void UserInterface::HideFromViewport()
{
	OnHidFromViewport.BroadCast();
	for (auto& widget : widgets)
	{
		widget->SetUIPattern(UIPattern::None);
	}
	for (auto& ui : userInterfaces)
	{
		ui->HideFromViewport();
	}
}

void UserInterface::RemoveFromViewport()
{
	if (bRemoveFlag)
	{
		return;
	}
	OnRemovedFromViewport.BroadCast();
	mainWorld.GameUIs_to_delete.insert(this);
	for (auto& ui : userInterfaces)
	{
		ui->RemoveFromViewport();
	}
	bRemoveFlag = true;
}

void UserInterface::AttachTo(UserInterface* aim)
{
	aim->userInterfaces.insert(this);
}

void UserInterface::DettachFrom(UserInterface* aim)
{
	aim->userInterfaces.erase(this);
}

void UserInterface::RegisterDontDestroy()
{
	for (auto& widget : widgets)
	{
		mainWorld.OverallRenders.insert(widget);
	}
	for (auto& ui : userInterfaces)
	{
		ui->RegisterDontDestroy();
	}
}