#include "SceneUI.h"

SceneUI::SceneUI()
{
	pointer = AddWidget<Image>();
	pointer->AttachTo(rootCanvas);
	pointer->SetLayoutPattern(LayoutPattern::Center);
	pointer->SetSize(Vector2D(170, 140));
	pointer->LoadSprite("pointer_sit");
	pointer->SetLayer(8);

	text = AddWidget<Text>();
	text->AttachTo(pointer);
	text->SetLayoutPattern(LayoutPattern::Center);
	text->SetSize(Vector2D(75, 25));
	text->SetRelativePosition(Vector2D(0, -15));
	text->SetLayer(8);
	text->SetText("$0REST", 5, "Trajan Pro");
}
