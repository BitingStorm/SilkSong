#include "BossNameUI.h"

BossNameUI::BossNameUI()
{
	text_ = AddWidget<Text>();
	text_->AttachTo(rootCanvas);
	text_->SetLayoutPattern(LayoutPattern::LeftBottom);
	text_->SetRelativePosition(FVector2D(200, -125));
	text_->SetLayer(8);
	text_->SetAlpha(0);

	text = AddWidget<Text>();
	text->AttachTo(text_);
	text->SetLayoutPattern(LayoutPattern::Center);
	text->SetRelativePosition(FVector2D(0, 40));
	text->SetLayer(8);
	text->SetAlpha(0);

	FadingTimerHandle.Bind(5.f, [this]() { flag = -1; });
}

void BossNameUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	alpha += flag * deltaTime * 150;
	alpha = FMath::Clamp(alpha, 0.f, 255.f);
	text_->SetAlpha(BYTE(alpha));
	text->SetAlpha(BYTE(alpha));
}

void BossNameUI::InitName(std::string name, std::string name_)
{
	text_->SetText(name_, 4, "Trajan Pro");
	text->SetText(name, 9, "Trajan Pro");
}
