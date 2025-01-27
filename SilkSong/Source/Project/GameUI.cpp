#include "GameUI.h"
#include "Player.h"
#include "GameplayStatics.h"


GameUI::GameUI()
{
	SoulContainer = AddWidget<Image>();
	SoulContainer->AttachTo(rootCanvas);
	SoulContainer->SetLayoutPattern(LayoutPattern::LeftTop);
	SoulContainer->SetRelativePosition(Vector2D(150, 75));
	SoulContainer->SetSize(Vector2D(172, 106));
	SoulContainer->LoadSprite("inventory_soul");
	SoulContainer->SetLayer(10);

	SoulContainer_ = AddWidget<Image>();
	SoulContainer_->AttachTo(SoulContainer);
	SoulContainer_->SetLayoutPattern(LayoutPattern::LeftTop);
	SoulContainer_->SetRelativePosition(Vector2D(50, 65));
	SoulContainer_->SetSize(Vector2D(106, 106));
	SoulContainer_->LoadSprite("inventory_soul_");
	SoulContainer_->SetLayer(11);


	
	Coin = AddWidget<Image>();
	Coin->AttachTo(SoulContainer);
	Coin->SetLayoutPattern(LayoutPattern::RightMiddle);
	Coin->SetRelativePosition(Vector2D(-40, 35));
	Coin->SetSize(Vector2D(47, 48));
	Coin->LoadSprite("inventory_coin");
	Coin->SetLayer(12);


	CoinNum = AddWidget<Text>();
	CoinNum->AttachTo(Coin);
	CoinNum->SetRelativePosition(Vector2D(65, 25));
	CoinNum->SetSize(Vector2D(100, 50));
	CoinNum->SetLayer(12);
	CoinNum->SetPattern(CharactersPattern::Left);

	Dart = AddWidget<Image>();
	Dart->AttachTo(SoulContainer);
	Dart->SetLayoutPattern(LayoutPattern::RightMiddle);
	Dart->SetRelativePosition(Vector2D(-40, 75));
	Dart->SetSize(Vector2D(47, 48));
	Dart->LoadSprite("dart");
	Dart->SetLayer(12);

	DartNum = AddWidget<Text>();
	DartNum->AttachTo(Dart);
	DartNum->SetRelativePosition(Vector2D(65, 25));
	DartNum->SetSize(Vector2D(100, 50));
	DartNum->SetLayer(12);
	DartNum->SetPattern(CharactersPattern::Left);

	for (int i = 0; i < 5; i++)
	{
		Health[i] = AddWidget<Image>();
		Health[i]->AttachTo(SoulContainer);
		Health[i]->SetLayoutPattern(LayoutPattern::RightMiddle);
		Health[i]->SetRelativePosition(Vector2D(45 * i - 45, -20));
		Health[i]->SetSize(Vector2D(34, 50));
		Health[i]->LoadSprite("inventory_health");
		Health[i]->SetLayer(12);
	}

	for (int i = 0; i < 5; i++)
	{
		Health_[i] = AddWidget<Image>();
		Health_[i]->AttachTo(SoulContainer);
		Health_[i]->SetLayoutPattern(LayoutPattern::RightMiddle);
		Health_[i]->SetRelativePosition(Vector2D(45 * i - 45, -20));
		Health_[i]->SetSize(Vector2D(34, 50));
		Health_[i]->LoadSprite("inventory_health_");
		Health_[i]->SetLayer(11);
	}
}

void GameUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	Player* player = Cast<Player>(GameplayStatics::GetController());
	for (int i = 0; i < player->GetHealth(); i++)
	{
		Health[i]->SetUIPattern(UIPattern::VisibleOnly);
	}
	for (int i = player->GetHealth(); i < 5; i++)
	{
		Health[i]->SetUIPattern(UIPattern::None);
	}

	SoulContainer_->SetStartAndEndLoc({ 0,0 }, { 100,100 - int(100 * player->GetSoul() / 9) });

	CoinNum->SetText("$0" + std::to_string(player->GetGeo()), 5, "Trajan Pro");

	DartNum->SetText("$0" + std::to_string(player->GetDart()), 5, "Trajan Pro");
}


