#include "SceneTitleUI.h"
#include "GameplayStatics.h"

SceneTitleUI::SceneTitleUI()
{
	Title = GameplayStatics::CreateUI<RichTextUI>();
	Title->AttachTo(this);
	Title->SetText("GROTTO", 12);
	Title->SetPosition(FVector2D(0, -150));
	Title->SetAlpha(0);

	Title_ = GameplayStatics::CreateUI<RichTextUI>();
	Title_->AttachTo(this);
	Title_->SetText("M O S S", 5);
	Title_->SetPosition(FVector2D(0, -200));
	Title_->SetAlpha(0);
	

	Top = AddWidget<Image>();
	Top->AttachTo(rootCanvas);
	Top->SetLayoutPattern(LayoutPattern::Center);
	Top->SetRelativePosition(FVector2D(0, -250));
	Top->SetLayer(30);
	Top->EnableAnimControl();
	topStart.Load("title_top");
	topStart.SetInterval(0.06f);
	topStart.SetLooping(false);
	topEnd.Load("title_top");
	topEnd.SetInterval(0.06f);
	topEnd.SetLooping(false);
	topEnd.SetReverse(true);
	Animator* ani = Top->GetAnimator();
	if (ani)
	{
		ani->Insert("start", topStart);
		ani->Insert("end", topEnd);
		ani->SetNode("start");
	}

	Bottom = AddWidget<Image>();
	Bottom->AttachTo(rootCanvas);
	Bottom->SetLayoutPattern(LayoutPattern::Center);
	Bottom->SetRelativePosition(FVector2D(0, -65));
	Bottom->SetLayer(30);
	Bottom->EnableAnimControl();
	bottomStart.Load("title_bottom");
	bottomStart.SetInterval(0.06f);
	bottomStart.SetLooping(false);
	bottomEnd.Load("title_bottom");
	bottomEnd.SetInterval(0.06f);
	bottomEnd.SetLooping(false);
	bottomEnd.SetReverse(true);
	ani = Bottom->GetAnimator();
	if (ani)
	{
		ani->Insert("start", bottomStart);
		ani->Insert("end", bottomEnd);
		ani->SetNode("start");
	}

	timer = 0.f;
	step = 150.f;
	DestroyTimerHandle.Bind(5.f, [this]()
		{
			step = -step * 2.f;
			Top->GetAnimator()->SetNode("end");
			Bottom->GetAnimator()->SetNode("end");
		});
}

void SceneTitleUI::Update(float deltaTime)
{
	UserInterface::Update(deltaTime);

	timer += step * deltaTime;
	timer = FMath::Clamp(timer, 0.f, 255.f);
	if (timer <= 0.f)
	{
		RemoveFromViewport();
	}
	Title->SetAlpha(timer);
	Title_->SetAlpha(timer);
}
