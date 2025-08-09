#include "EscUI.h"
#include "GameplayStatics.h"
#include "Pointer.h"
#include "Effect.h"
#include "GameModeHelper.h"
#include "MenuLevel.h"
#include "Player.h"


std::string texts_esc[2] = {
	"CONTINUE",
	"BACK TO MENU",
};

EscUI::EscUI()
{
	Black = AddWidget<Image>();
	Black->AttachTo(rootCanvas);
	Black->SetLayoutPattern(LayoutPattern::Center);
	Black->LoadSprite("black");
	Black->SetLayer(30);
	Black->SetTransparency(0);



	float delta[2] = { 115,80 };
	for (int i = 0; i < 2; ++i)
	{
		Options[i] = AddWidget<Text>();
		Options[i]->AttachTo(rootCanvas);
		Options[i]->SetLayoutPattern(LayoutPattern::Center);
		Options[i]->SetRelativePosition(FVector2D(0, -50 + 75 * i));
		Options[i]->SetLayer(40);
		Options[i]->SetText(texts_esc[i], 5, "Trajan Pro");

		Buttons[i] = AddWidget<Button>();
		Buttons[i]->AttachTo(Options[i]);
		Buttons[i]->SetLayoutPattern(LayoutPattern::Center);
		Buttons[i]->SetSize(FVector2D(100, 50));
		Buttons[i]->SetLayer(41);

		FVector2D pos = GameplayStatics::ProjectScreenToWorld(Buttons[i]->GetScreenPosition());
		Buttons[i]->OnMouseHoverBegin.AddLambda([=]() {
			GameplayStatics::CreateObject<Pointer>(pos + FVector2D(delta[i], 0))->SetLocalScale(FVector2D(-1, 1));
			GameplayStatics::CreateObject<Pointer>(pos - FVector2D(delta[i], 0));
			GameModeHelper::PlayFXSound("sound_change_selection");
			});
		Buttons[i]->OnMouseHoverEnd.AddLambda([=]() {
			for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
			{
				obj->FadeOut();
			}
			});
		Buttons[i]->OnMousePressedBegin.AddLambda([=]() {
			Effect* effect = GameplayStatics::CreateObject<Effect>(pos);
			effect->SetLocalScale(FVector2D(0.75, 0.75));
			effect->Init("menuhit", -0.04f);
			
			for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
			{
				obj->FadeOut();
			}
			if (i == 1) GameModeHelper::PlayFXSound("sound_save");
			GameModeHelper::PlayFXSound("sound_button_confirm");
			});
	}

	Buttons[0]->OnMousePressedBegin.AddLambda([this]() {
		HideFromViewport();
		});
	Buttons[1]->OnMousePressedBegin.AddLambda([this]() {
		GameplayStatics::GetController()->Destroy();
		GameplayStatics::OpenLevel("Menu");
		});

	OnAddedToViewport.AddLambda([this]() {Black->SetTransparency(100); });
	OnHidFromViewport.AddLambda([this]() {Black->SetTransparency(0); });
}
