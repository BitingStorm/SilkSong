#include "RichTextUI.h"

RichTextUI::RichTextUI()
{
	Others = AddWidget<Text>();
	Others->AttachTo(rootCanvas);
	Others->SetLayoutPattern(LayoutPattern::Center);
	Others->SetLayer(30);

	First = AddWidget<Text>();
	First->AttachTo(Others);
	First->SetLayoutPattern(LayoutPattern::LeftMiddle);
	First->SetLayer(30);
}

void RichTextUI::SetText(std::string text, int size)
{
	float delta = FMath::Ceil(0.2f * size);
	First->SetText(text.substr(0, 1), size + delta, "Trajan Pro");
	Others->SetText(text.substr(1), size, "Trajan Pro");
	float width = TextHelper::GetWidthOfOneLetter(size + delta, "Trajan Pro", (First->GetText())[0]) * 0.5f;
	First->SetRelativePosition(FVector2D(-width, 0));
	Others->SetRelativePosition(FVector2D(width, 0));
}

void RichTextUI::SetAlpha(BYTE alpha)
{
	First->SetAlpha(alpha);
	Others->SetAlpha(alpha);
}

void RichTextUI::SetPosition(FVector2D pos)
{
	Others->SetRelativePosition(pos + Others->GetRelativePosition());
}
