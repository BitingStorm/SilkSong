#include "BossNameUI.h"
#include "GameplayStatics.h"

BossNameUI::BossNameUI()
{
	text_ = GameplayStatics::CreateUI<RichTextUI>();
	text_->AttachTo(this);
	text_->AddToViewport();
	text_->SetAlpha(0);

	text = GameplayStatics::CreateUI<RichTextUI>();
	text->AttachTo(this);
	text->AddToViewport();
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
	text_->SetText(name_, 4);
	text->SetText(name, 9);
	text_->SetPosition(FVector2D(-400, 260));
	text->SetPosition(FVector2D(-400, 300));
}
