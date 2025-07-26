#include "VolumeBarUI.h"
#include "GameModeHelper.h"
#include "GameplayStatics.h"
#include "Objects/Controller.h"


std::string Texts[2] = { "MUSIC  VOLUME :","SOUND  VOLUME :" };


VolumeBarUI::VolumeBarUI()
{
	VolumeText = GameplayStatics::CreateUI<RichTextUI>();
	VolumeText->AttachTo(this);

	VolumeBar = AddWidget<Bar>();
	VolumeBar->AttachTo(rootCanvas);
	VolumeBar->SetLayoutPattern(LayoutPattern::Center);
	VolumeBar->LoadBarBackPicture("white");
	VolumeBar->LoadBarFrontPicture("bar_white");
	VolumeBar->SetSize({ 200.f,5.f });
	VolumeBar->SetFrontSize({ 198,3 });
	VolumeBar->SetBackSize({ 200,5 });
	VolumeBar->SetLayer(10);

	VolumeButton = AddWidget<Button>();
	VolumeButton->AttachTo(VolumeBar);
	VolumeButton->SetLayoutPattern(LayoutPattern::Center);
	VolumeButton->SetSize({ 75,50 });
	VolumeButton->SetLayer(11);
	VolumeButton->LoadNormalPicture("menu_slider");

	VolumeNumber = AddWidget<Text>();
	VolumeNumber->AttachTo(VolumeBar);
	VolumeNumber->SetLayoutPattern(LayoutPattern::RightMiddle);
	VolumeNumber->SetRelativePosition(FVector2D(100, 0));
	VolumeNumber->SetLayer(10);
}

void VolumeBarUI::Init(int32 i)
{
	VolumeText->SetText(Texts[i], 5);
	VolumeText->SetPosition(FVector2D(-175, -150 + i * 100));
	VolumeBar->SetRelativePosition(FVector2D(165, -150 + i * 100));
	index = i;

	float delta_x = float(GameModeHelper::GetInstance()->GetVolume(index)) / 5 - 100;
	VolumeButton->SetRelativePosition(FVector2D(delta_x, -30));
}

void VolumeBarUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	GameMode* mode = GameModeHelper::GetInstance();

	if (VolumeButton->IsMousePressed())
	{
		float delta_x = FMath::Clamp(GameplayStatics::ProjectWorldToScreen(GameplayStatics::GetController()->GetCursorPosition()).x 
			- VolumeBar->GetScreenPosition().x, -100.f, 100.f);
		VolumeButton->SetRelativePosition(FVector2D(delta_x, -30));

		delta_x += 100;

		mode->SetVolume(index, 5 * delta_x);
	}
	VolumeBar->SetPercentage(float(mode->GetVolume(index)) / 1000.f);
	VolumeNumber->SetText(std::to_string(mode->GetVolume(index) / 100), 5, "Trajan Pro");
}