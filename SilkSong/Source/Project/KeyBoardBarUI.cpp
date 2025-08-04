#include "KeyBoardBarUI.h"
#include "GameplayStatics.h"
#include "Pointer.h"
#include "Effect.h"
#include "GameModeHelper.h"
#include "KeyBoardUI.h"
#include "ButtonIconUI.h"


std::string actionTexts[15] = { "ÉÏ","×ó","ÏÂ","ÓÒ","ÌøÔ¾","¹¥»÷","³å´Ì","ÉÁ±Ü","ÒøË¿ÂÒÎè","»ØÏìÖ®Õë/¹³Ëø","Õİ´ÌË²Ï®","¾Û¼¯","¸ñµ²","·ÉïÚ","¼²ÅÜ"};


KeyBoardBarUI::KeyBoardBarUI()
{
	ActionText = AddWidget<Text>();
	ActionText->AttachTo(rootCanvas);
	ActionText->SetLayoutPattern(LayoutPattern::LeftMiddle);
	ActionText->SetLayer(30);
	ActionText->SetRelativePosition(FVector2D(50, 0));

	ChangeButton = AddWidget<Button>();
	ChangeButton->AttachTo(rootCanvas);
	ChangeButton->SetLayoutPattern(LayoutPattern::Center);
	ChangeButton->SetSize({ 300,60 });
	ChangeButton->SetLayer(31);

	SettingText = AddWidget<Text>();
	SettingText->AttachTo(rootCanvas);
	SettingText->SetLayoutPattern(LayoutPattern::RightMiddle);
	SettingText->SetLayer(30);
	SettingText->SetRelativePosition(FVector2D(-50, 0));
	SettingText->SetText("Éè¶¨¼üÎ»", 5);

	Panel = AddWidget<VerticalPanel>();
	Panel->AttachTo(rootCanvas);
	Panel->SetLayoutPattern(LayoutPattern::RightMiddle);
	Panel->SetRelativePosition(FVector2D(-50, 0));
	Panel->SetUnitSize(FVector2D(60, 60));

	ChangeButton->OnMouseHoverBegin.AddLambda([this]() {
		if (SettingText->GetAlpha() != 0)
		{
			return;
		}
		FVector2D pos = GameplayStatics::ProjectScreenToWorld(ChangeButton->GetScreenPosition());
		GameplayStatics::CreateObject<Pointer>(pos + FVector2D(185, 0))->SetLocalScale(FVector2D(-1, 1));
		GameplayStatics::CreateObject<Pointer>(pos - FVector2D(185, 0));
		GameModeHelper::PlayFXSound("sound_change_selection");
		});
	ChangeButton->OnMouseHoverEnd.AddLambda([]() {
		for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
		{
			obj->FadeOut();
		}
		});
	ChangeButton->OnMousePressedBegin.AddLambda([this]() {
		if (SettingText->GetAlpha() != 0)
		{
			return;
		}
		FVector2D pos = GameplayStatics::ProjectScreenToWorld(ChangeButton->GetScreenPosition());
		Effect* effect = GameplayStatics::CreateObject<Effect>(pos);
		effect->SetLocalScale(FVector2D(0.75, 0.75));
		effect->Init("menuhit", -0.04f);

		for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
		{
			obj->FadeOut();
		}
		GameModeHelper::PlayFXSound("sound_button_confirm");
		});
	ChangeButton->OnMousePressedEnd.AddLambda([this]() {
		if (SettingText->GetAlpha() != 0)
		{
			return;
		}
		SettingText->SetAlpha(255);
		GameplayStatics::FindUIOfClass<KeyBoardUI>()->SetCurrentBar(this);
		if (Icon)
		{
			Panel->RemoveMember(Icon);
			Icon->RemoveFromViewport();
			Icon = nullptr;
		}
		});
}

void KeyBoardBarUI::Init(int i)
{
	ActionText->SetText(actionTexts[i], 4);
	index = i;
}

bool KeyBoardBarUI::SetIcon(uint8 keyCode)
{
	std::cout << int(keyCode);
	if ((keyCode >= 0x25 && keyCode <= 0x28) || (keyCode <= 0x04) && (keyCode != 0x03))
	{
		Icon = GameplayStatics::CreateUI<ButtonIconImageUI>();
	}
	else if ((keyCode >= 0x30 && keyCode <= 0x39) || (keyCode >= 0x41 && keyCode <= 0x5A))
	{
		Icon = GameplayStatics::CreateUI<ButtonIconCharUI>();
	}
	else if (ButtonIconUI::IsCharLegal(keyCode))
	{
		Icon = GameplayStatics::CreateUI<ButtonIconCharUI>();
		Cast<ButtonIconCharUI>(Icon)->EnableFlag();
	}
	else if (ButtonIconUI::IsStringLegal(keyCode))
	{
		Icon = GameplayStatics::CreateUI<ButtonIconStringUI>();
	}
	else
	{
		return false;
	}

	SettingText->SetAlpha(0);
	CHECK_PTR(Icon)
	
	
	Icon->SetIcon(keyCode);
	Panel->AddMember(Icon);
	Icon->AddToViewport();

	return true;
}
