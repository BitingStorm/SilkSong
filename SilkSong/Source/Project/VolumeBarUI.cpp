#include "VolumeBarUI.h"
#include "GameModeHelper.h"
#include "GameplayStatics.h"
#include "Objects/Controller.h"


std::string Texts[2] = { "$0MUSIC VOLUME:","$0SOUND VOLUME:" };


VolumeBarUI::VolumeBarUI()
{
	VolumeText = AddWidget<Text>();
	VolumeText->AttachTo(rootCanvas);
	VolumeText->SetLayoutPattern(LayoutPattern::MiddleTop);
	VolumeText->SetRelativePosition(FVector2D(-150, 250));
	VolumeText->SetLayer(10);

	VolumeBar = AddWidget<Bar>();
	VolumeBar->AttachTo(VolumeText);
	VolumeBar->SetLayoutPattern(LayoutPattern::RightMiddle);
	VolumeBar->SetRelativePosition(FVector2D(200, 0));
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
	VolumeText->SetRelativePosition(FVector2D(-150, 250 + i * 100));
	VolumeText->SetText(Texts[i], 5, "Trajan Pro");
	index = i;

	float delta_x = float(GameModeHelper::GetInstance()->GetAudioPlayer(index)->GetVolume()) / 5 - 100;
	VolumeButton->SetRelativePosition(FVector2D(delta_x, -30));
}

void VolumeBarUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	AudioPlayer* audio = GameModeHelper::GetInstance()->GetAudioPlayer(index);

	if (VolumeButton->IsMousePressed())
	{
		float delta_x = FMath::Clamp(GameplayStatics::ProjectWorldToScreen(GameplayStatics::GetController()->GetCursorPosition()).x 
			- VolumeBar->GetScreenPosition().x, -100.f, 100.f);
		VolumeButton->SetRelativePosition(FVector2D(delta_x, -30));

		delta_x += 100;

		audio->SetVolume(5 * delta_x);
	}
	VolumeBar->SetPercentage(float(audio->GetVolume()) / 1000.f);
	VolumeNumber->SetText("$0" + std::to_string(audio->GetVolume() / 100), 5, "Trajan Pro");
}