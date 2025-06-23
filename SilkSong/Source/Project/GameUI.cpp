#include "GameUI.h"
#include "Player.h"
#include "GameplayStatics.h"
#

GameUI::GameUI()
{
	White = AddWidget<Image>();
	White->AttachTo(rootCanvas);
	White->SetLayoutPattern(LayoutPattern::Center);
	White->LoadSprite("white");
	White->SetLayer(9);
	White->SetTransparency(0);

	LowHealth = AddWidget<Image>();
	LowHealth->AttachTo(rootCanvas);
	LowHealth->SetLayoutPattern(LayoutPattern::Center);
	LowHealth->LoadSprite("low_health");
	LowHealth->SetLayer(9);
	LowHealth->SetTransparency(0);

	Black = AddWidget<Image>();
	Black->AttachTo(rootCanvas);
	Black->SetLayoutPattern(LayoutPattern::Center);
	Black->LoadSprite("black");
	Black->SetLayer(20);
	Black->SetTransparency(0);

	SoulContainer = AddWidget<Image>();
	SoulContainer->AttachTo(rootCanvas);
	SoulContainer->SetRelativePosition(FVector2D(100, 75));
	SoulContainer->LoadSprite("inventory_ins1");
	SoulContainer->SetLayer(11);

	SoulContainer_ = AddWidget<Image>();
	SoulContainer_->AttachTo(SoulContainer);
	SoulContainer_->SetLayoutPattern(LayoutPattern::RightMiddle);
	SoulContainer_->SetRelativePosition(FVector2D(75, 0));
	SoulContainer_->LoadSprite("inventory_ins2");
	SoulContainer_->SetLayer(10);

	SilkContainer = AddWidget<Image>();
	SilkContainer->AttachTo(SoulContainer);
	SilkContainer->SetLayoutPattern(LayoutPattern::MiddleBottom);
	SilkContainer->SetRelativePosition(FVector2D(25, 35));
	SilkContainer->LoadSprite("inventory_ins3");
	SilkContainer->SetLayer(11);

	Coin = AddWidget<Image>();
	Coin->AttachTo(SoulContainer);
	Coin->SetLayoutPattern(LayoutPattern::MiddleBottom);
	Coin->SetRelativePosition(FVector2D(0, 100));
	Coin->LoadSprite("inventory_coin");
	Coin->SetLayer(12);

	CoinNum = AddWidget<Text>();
	CoinNum->AttachTo(Coin);
	CoinNum->SetLayoutPattern(LayoutPattern::RightMiddle);
	CoinNum->SetRelativePosition(FVector2D(25, 0));
	CoinNum->SetSize(FVector2D(100, 50));
	CoinNum->SetLayer(12);
	CoinNum->SetPattern(CharactersPattern::Left);

	ItemSector = AddWidget<Sector>();
	ItemSector->AttachTo(SilkContainer);
	ItemSector->SetRelativePosition(FVector2D(100, 0));
	ItemSector->SetLayer(12);
	ItemSector->SetLayoutPattern(LayoutPattern::Center);
	ItemSector->LoadSectorFrontPicture("inventory_item");
	ItemSector->LoadSectorBackPicture("inventory_item_");

	Dart = AddWidget<Image>();
	Dart->AttachTo(ItemSector);
	Dart->SetLayoutPattern(LayoutPattern::Center);
	Dart->SetRelativePosition(FVector2D(4, 0));
	Dart->SetSize(FVector2D(47, 48));
	Dart->LoadSprite("dart");
	Dart->SetLayer(12);
	
	for (int i = 0; i < 5; i++)
	{
		bloodidle[i].Load("inventory_bloodidle");
		bloodidle[i].SetInterval(0.06f);
		bloodidle[i].SetLooping(false);
		bloodload[i].Load("inventory_bloodload");
		bloodload[i].SetInterval(0.1f);
		bloodload[i].SetLooping(false);
		bloodminus[i].Load("inventory_bloodminus");
		bloodminus[i].SetInterval(0.07f);
		bloodminus[i].SetLooping(false);

		Health[i] = AddWidget<Image>();
		Health[i]->AttachTo(SoulContainer);
		Health[i]->SetLayoutPattern(LayoutPattern::RightMiddle);
		Health[i]->SetRelativePosition(FVector2D(45 * i+35, 0));
		Health[i]->SetLayer(12);

		Health[i]->EnableAnimControl();
		Animator* ani = Health[i]->GetAnimator();
		if (!ani)continue;
		ani->Insert("idle", bloodidle[i]);
		ani->Insert("load", bloodload[i]);
		ani->Insert("minus", bloodminus[i]);
		ani->SetNode("load");
	}

	IdleBlink.Bind(2.5f, [this]() {
		Player* player = Cast<Player>(GameplayStatics::GetController());
		for (int i = 0; i < player->GetHealth(); i++)
		{
			Health[i]->GetAnimator()->SetNode("idle");
		}
		},true);

	for (int i = 0; i < 9; i++)
	{
		silkload[i].Load("inventory_silk");
		silkload[i].SetInterval(0.06f);
		silkload[i].SetLooping(false);
		silkminus[i].Load("inventory_silk");
		silkminus[i].SetInterval(0.06f);
		silkminus[i].SetLooping(false);
		silkminus[i].SetReverse(true);
	
		Silk[i] = AddWidget<Image>();
		Silk[i]->AttachTo(SilkContainer);
		Silk[i]->SetLayoutPattern(LayoutPattern::LeftMiddle);
		Silk[i]->SetRelativePosition(FVector2D(8 * i + 21, 0));
		Silk[i]->SetLayer(12);

		Silk[i]->EnableAnimControl();
		Animator* ani = Silk[i]->GetAnimator();
		ani->Insert("load", silkload[i]);
		ani->Insert("minus", silkminus[i]);
		ani->SetNode("load");
	}

	soulload.Load("inventory_soul");
	soulload.SetInterval(0.06f);
	soulload.SetLooping(false);
	soulminus.Load("inventory_soul");
	soulminus.SetInterval(0.06f);
	soulminus.SetLooping(false);
	soulminus.SetReverse(true);

	Soul = AddWidget<Image>();
	Soul->AttachTo(SoulContainer);
	Soul->SetLayoutPattern(LayoutPattern::Center);
	Soul->SetRelativePosition(FVector2D(0, 0));
	Soul->SetLayer(12);

	Soul->EnableAnimControl();
	Animator* ani = Soul->GetAnimator();
	ani->Insert("load", soulload);
	ani->Insert("minus", soulminus);
	ani->SetNode("load");
}

void GameUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	if (BYTE trans = White->GetTransparency())
	{
		White->SetTransparency(trans - blinkStep);
	}
	
	Player* player = Cast<Player>(GameplayStatics::GetController());
	
	if (player->GetHealth() == 1)
	{
		lowHealthFlag += deltaTime * lowHealthDir * 100;
		if (FMath::Abs(lowHealthFlag - 150) > 100)
		{
			lowHealthDir = -lowHealthDir;
			lowHealthFlag += deltaTime * lowHealthDir * 100;
		}
	}
	else if (player->GetHealth() == 0)
	{
		lowHealthDir = -1;
		lowHealthFlag += deltaTime * lowHealthDir * 200;
	}
	else
	{
		lowHealthFlag = 75;
	}
	LowHealth->SetTransparency(player->GetHealth() <= 1 ? BYTE(lowHealthFlag < 0 ? 0 : lowHealthFlag) : 0);

	if (bRecover)
	{
		if (Black->GetTransparency() < 250)Black->SetTransparency(Black->GetTransparency() + 25);
	}
	else
	{
		if (Black->GetTransparency() > 0)Black->SetTransparency(Black->GetTransparency() - 5);
	}

	CoinNum->SetText("$0" + std::to_string(player->GetGeo()), 5, "Trajan Pro");

	ItemSector->SetPercentage(float(player->GetDart())/15);
}

void GameUI::WhiteBlink(int32 step)
{
	White->SetTransparency(240);
	blinkStep = step;
}

void GameUI::BloodLoad(int i)
{
	Health[i]->GetAnimator()->SetNode("load");
	IdleBlink.Reset();
}

void GameUI::BloodMinus(int i)
{
	Health[i]->GetAnimator()->SetNode("minus");
	IdleBlink.Reset();
}

void GameUI::SilkLoad(int i)
{
	Silk[i]->GetAnimator()->SetNode("load");
}

void GameUI::SilkMinus(int i)
{
	Silk[i]->GetAnimator()->SetNode("minus");
}

void GameUI::SoulLoad()
{
	Soul->GetAnimator()->SetNode("load");
}

void GameUI::SoulMinus()
{
	Soul->GetAnimator()->SetNode("minus");
}
