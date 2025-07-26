#include "VolumeUI.h"
#include "Pointer.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"
#include "Effect.h"
#include "MenuUI.h"
#include "VolumeBarUI.h"


VolumeUI::VolumeUI()
{
	Title = GameplayStatics::CreateUI<RichTextUI>();
	Title->AttachTo(this);
	Title->SetText("AUDIO", 7);
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
	if(ani)
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


	Back->OnMouseHoverBegin.AddLambda([this]() {
		FVector2D pos = GameplayStatics::ProjectScreenToWorld(Back->GetScreenPosition());
		GameplayStatics::CreateObject<Pointer>(pos + FVector2D(75, 0))->SetLocalScale(FVector2D(-1, 1));
		GameplayStatics::CreateObject<Pointer>(pos - FVector2D(75, 0));
		GameModeHelper::PlayFXSound("sound_change_selection");
		});
	Back->OnMouseHoverEnd.AddLambda([]() {
		for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
		{
			obj->FadeOut();
		}
		});
	Back->OnMousePressedBegin.AddLambda([this]() {
		FVector2D pos = GameplayStatics::ProjectScreenToWorld(Back->GetScreenPosition());
		Effect* effect = GameplayStatics::CreateObject<Effect>(pos);
		effect->SetLocalScale(FVector2D(0.75, 0.75));
		effect->Init("menuhit", -0.03f);
		GameModeHelper::PlayFXSound("sound_button_confirm");
		HideFromViewport();
		menuUI->AddToViewport();
		for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
		{
			obj->FadeOut();
		}
		});

	for (int i = 0; i < 2; i++)
	{
		volumeBarUI[i] = GameplayStatics::CreateUI<VolumeBarUI>();
		volumeBarUI[i]->Init(i);
		volumeBarUI[i]->AttachTo(this);
	}
}

void VolumeUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);
}
