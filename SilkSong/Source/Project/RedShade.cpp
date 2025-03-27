#include "RedShade.h"
#include "Components/SpriteRenderer.h"
#include "Bg.h"
#include "GameplayStatics.h"

RedShade::RedShade()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->SetLayer(8);
	render->LoadSprite("red");

	GameplayStatics::CreateObject<Bg>({ -1050,300 }, 0.f, { -1.f, 1.f })->Init("grimmtent_bg", -5, true);

	GameplayStatics::CreateObject<Bg>({ -500.f,875.f })->Init("grimmtent_stand", -2);
	GameplayStatics::CreateObject<Bg>({ -1700.f,875.f }, 0.f, { -1.f,1.f })->Init("grimmtent_stand", -2);
	GameplayStatics::CreateObject<Bg>({ -1300.f,925.f }, 0.f, { -0.8f,0.8f })->Init("grimmtent_stand", -3);
	GameplayStatics::CreateObject<Bg>({ -900.f,925.f }, 0.f, { 0.8f,0.8f })->Init("grimmtent_stand", -3);
	GameplayStatics::CreateObject<Bg>({ -775.f,875.f })->Init("grimmtent_pole", -1);
	GameplayStatics::CreateObject<Bg>({ -1400.f,875.f })->Init("grimmtent_pole", -1);
	GameplayStatics::CreateObject<Bg>({ -1075.f,875.f }, 0.f, { 0.8f,0.8f })->Init("grimmtent_pole", -2);
	GameplayStatics::CreateObject<Bg>({ -1250.f,250.f })->Init("grimmtent_flag", -1, true, false);
	GameplayStatics::CreateObject<Bg>({ -800.f,235.f })->Init("grimmtent_flag", 2);
	GameplayStatics::CreateObject<Bg>({ -1100.f,230.f }, 0.f, { 0.8f,0.8f })->Init("grimmtent_flag", -7, true, false);
}

void RedShade::Update(float deltaTime)
{
	Actor::Update(deltaTime);
	
	alpha -= deltaTime * 200.f;
	if (alpha < 0)
	{
		Destroy();
	}
	render->SetTransparency(alpha);
}
