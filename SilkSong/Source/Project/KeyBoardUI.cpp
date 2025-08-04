#include "KeyBoardUI.h"
#include "Pointer.h"
#include "GameplayStatics.h"
#include "Objects/Controller.h"
#include "GameModeHelper.h"
#include "Effect.h"
#include "MenuUI.h"
#include "KeyBoardBarUI.h"


KeyBoardUI::KeyBoardUI()
{
	Title = GameplayStatics::CreateUI<RichTextUI>();
	Title->AttachTo(this);
	Title->SetText("KEYBOARD", 7);
	Title->SetPosition(FVector2D(0, -300));

	Warning = AddWidget<Image>();
	Warning->AttachTo(rootCanvas);
	Warning->SetLayoutPattern(LayoutPattern::Center);
	Warning->SetRelativePosition(FVector2D(0, -225));
	Warning->SetLayer(10);
	Warning->EnableAnimControl();
	idle.Load("menu_warning");
	idle.SetInterval(0.05f);
	idle.SetLooping(false);
	Animator* ani = Warning->GetAnimator();
	if (ani)
	{
		ani->Insert("idle", idle);
		ani->SetNode("idle");
	}
	OnAddedToViewport.AddLambda([this]() {idle.SetIndex(0); });

	Back = AddWidget<Button>();
	Back->AttachTo(rootCanvas);
	Back->SetLayoutPattern(LayoutPattern::MiddleBottom);
	Back->SetRelativePosition(FVector2D(0, -100));
	Back->SetSize({ 100,50 });
	Back->SetLayer(11);

	BackText = GameplayStatics::CreateUI<RichTextUI>();
	BackText->AttachTo(this);
	BackText->SetText("BACK", 5);
	BackText->SetPosition(FVector2D(0, 300));

	KeyBoardPanel = AddWidget<GridPanel>();
	KeyBoardPanel->AttachTo(rootCanvas);
	KeyBoardPanel->SetLayoutPattern(LayoutPattern::Center);
	KeyBoardPanel->SetRelativePosition(FVector2D(0, 40));
	KeyBoardPanel->SetRow(8);
	KeyBoardPanel->SetColumn(2);
	KeyBoardPanel->SetUnitSize(FVector2D(300,60));
	KeyBoardPanel->SetSpacingX(100);
	for (int32 i = 0; i < 15; ++i)
	{
		KeyBoardBarUI* bar = GameplayStatics::CreateUI<KeyBoardBarUI>();
		KeyBoardPanel->AddMember(bar, i);
		bar->Init(i);
		bar->SetIcon(GameModeHelper::GetInstance()->GetKeyCode(i));
		bar->HideFromViewport();
	}

	Back->OnMouseHoverBegin.AddLambda([this]() {
		if (currentBar)
		{
			return;
		}
		FVector2D pos = GameplayStatics::ProjectScreenToWorld(Back->GetScreenPosition());
		GameplayStatics::CreateObject<Pointer>(pos + FVector2D(80, 0))->SetLocalScale(FVector2D(-1, 1));
		GameplayStatics::CreateObject<Pointer>(pos - FVector2D(80, 0));
		GameModeHelper::PlayFXSound("sound_change_selection");
		});
	Back->OnMouseHoverEnd.AddLambda([]() {
		for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
		{
			obj->FadeOut();
		}
		});
	Back->OnMousePressedBegin.AddLambda([this]() {
		if (currentBar)
		{
			return;
		}
		FVector2D pos = GameplayStatics::ProjectScreenToWorld(Back->GetScreenPosition());
		Effect* effect = GameplayStatics::CreateObject<Effect>(pos);
		effect->SetLocalScale(FVector2D(0.75, 0.75));
		effect->Init("menuhit", -0.03f);
		GameModeHelper::PlayFXSound("sound_button_confirm");
		GameplayStatics::FindUIOfClass<MenuUI>()->AddToViewport();
		HideFromViewport();
		for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
		{
			obj->FadeOut();
		}
		});
}

void KeyBoardUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	if (currentBar && GameplayStatics::GetController()->IsAnyKeyPressed())
	{
		uint8 keyCode = GameplayStatics::GetController()->GetCurrentKeyCode();
		if (!currentBar->SetIcon(keyCode))
		{
			return;
		}
		GameModeHelper::GetInstance()->SetKeyCode(currentBar->GetIndex(), keyCode);
		currentBar = nullptr;
	}
}
