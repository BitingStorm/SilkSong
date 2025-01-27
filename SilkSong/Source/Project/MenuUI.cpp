#include "MenuUI.h"
#include "GameplayStatics.h"
#include "Pointer.h"
#include "Effect.h"


MenuUI::MenuUI()
{
	Title = AddWidget<Image>();
	Title->AttachTo(rootCanvas);
	Title->SetLayoutPattern(LayoutPattern::Center);
	Title->SetRelativePosition(Vector2D(0, -175));
	Title->SetSize(Vector2D(700, 275));
	Title->LoadSprite("menu_title");
	Title->SetLayer(10);

	Logo = AddWidget<Image>();
	Logo->AttachTo(rootCanvas);
	Logo->SetLayoutPattern(LayoutPattern::LeftBottom);
	Logo->SetRelativePosition(Vector2D(200, -175));
	Logo->SetSize(Vector2D(105, 30));
	Logo->LoadSprite("menu_logo");
	Logo->SetLayer(10);

	Edition = AddWidget<Text>();
	Edition->AttachTo(Logo);
	Edition->SetLayoutPattern(LayoutPattern::MiddleBottom);
	Edition->SetRelativePosition(Vector2D(0, 20));
	Edition->SetSize(Vector2D(75, 25));
	Edition->SetLayer(10);
	Edition->SetText("$01.5.78.11833",2);

	Icon = AddWidget<Image>();
	Icon->AttachTo(rootCanvas);
	Icon->SetLayoutPattern(LayoutPattern::RightBottom);
	Icon->SetRelativePosition(Vector2D(-150, -175));
	Icon->SetSize(Vector2D(125, 101));
	Icon->LoadSprite("menu_icon");
	Icon->SetLayer(10);

	float delta[5] = {115,90,130,80,110};

	for (int i = 0; i < 5; i++)
	{
		Buttons[i] = AddWidget<Button>();
		Buttons[i]->AttachTo(Title);
		Buttons[i]->SetLayoutPattern(LayoutPattern::MiddleBottom);
		Buttons[i]->SetRelativePosition(Vector2D(0, 50 * i + 50));
		Buttons[i]->SetSize(Vector2D(delta[i]*1.5f, 50));
		Buttons[i]->SetLayer(12);

		Vector2D pos = Buttons[i]->GetScreenPosition() - Vector2D(WIN_WIDTH * 0.5, WIN_HEIGHT * 0.5);
		Buttons[i]->OnMouseHoverBegin.AddLambda([=]() {
			GameplayStatics::CreateObject<Pointer>(pos + Vector2D(delta[i], 0))->SetLocalScale(Vector2D(-1, 1));
			GameplayStatics::CreateObject<Pointer>(pos - Vector2D(delta[i], 0));
			GameplayStatics::PlaySound2D("sound_change_selection");
			});
		Buttons[i]->OnMouseHoverEnd.AddLambda([=]() {
			for (auto& obj : GameplayStatics::FindObjectsOfClass<Pointer>())
			{
				obj->FadeOut();
			}
			});
		Buttons[i]->OnMousePressedBegin.AddLambda([=]() {
			Effect* effect = GameplayStatics::CreateObject<Effect>(pos);
			effect->SetLocalScale(Vector2D(0.75, 0.75));
			effect->Init("menuhit",-0.02f);
			GameplayStatics::PlaySound2D("sound_button_confirm");
			});

		Options[i] = AddWidget<Text>();
		Options[i]->AttachTo(Buttons[i]);
		Options[i]->SetLayoutPattern(LayoutPattern::Center);
		Options[i]->SetSize(Vector2D(150, 40));
		Options[i]->SetLayer(10);
		Options[i]->SetPattern(CharactersPattern::Middle);
		Options[i]->SetRelativePosition(Vector2D(-4, 2));
	}


	Options[0]->SetText("$0START GAME", 5, "Trajan Pro");
	Options[1]->SetText("$0OPTIONS", 5, "Trajan Pro");
	Options[2]->SetText("$0ACHIEVEMENTS", 5, "Trajan Pro");
	Options[3]->SetText("$0EXTRAS", 5, "Trajan Pro");
	Options[4]->SetText("$0QUIT GAME", 5, "Trajan Pro");

	
	Buttons[0]->OnMousePressedBegin.AddLambda([]() {GameplayStatics::OpenLevel("TearCity");});
	Buttons[4]->OnMousePressedBegin.AddLambda([]() { GameplayStatics::QuitGame(); });
}

void MenuUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);
}