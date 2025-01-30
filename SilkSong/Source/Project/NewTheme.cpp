#include "NewTheme.h"
#include "GameplayStatics.h"
#include "Components/ParticleSystem.h"
#include "Bg.h"


NewTheme::NewTheme()
{
	for (int i = 0; i < 4; i++)
	{
		particles[i] = ConstructComponent<ParticleSystem>();
		particles[i]->SetLocalPosition({ -200,500 });
		particles[i]->AttachTo(root);
		particles[i]->SetCapacity(27 - i * 2);
		particles[i]->SetInterval(0.12f - i * 0.008f);
		particles[i]->Load("menu_ss_particle");
		particles[i]->SetMaxSpeed(275 + i * 70);
		particles[i]->SetMinSpeed(150 + i * 70);
		particles[i]->SetPattern(ParticlePattern::Line);
		particles[i]->SetLine(800, 80 - 25 * (i % 2));
		particles[i]->SetLayer(i);
		particles[i]->SetLifeCycle(3.3f - i * 0.45);
		particles[i]->SetFadingOutTime(0.5f);
		particles[i]->SetFadingInTime(0.25f);
		particles[i]->SetFadingType(ParticleFadingType::FadeInAndOut);
		particles[i]->SetGravity(0);
		particles[i]->SetSizeRange(0.15 + i * 0.1, 0.2 + i * 0.1);
	}
	particles[2]->SetLine(800, 95);
	particles[2]->SetLocalPosition({ 0,500 });
	particles[3]->SetCapacity(10);
	particles[3]->SetInterval(0.15);
	particles[3]->SetSizeRange(0.6, 0.9);
	particles[3]->SetMaxSpeed(650);
	particles[3]->SetMinSpeed(550);
	particles[3]->SetLayer(7);
	particles[3]->SetLine(1000, 65);

	Bg* bg = GameplayStatics::CreateObject<Bg>({ 0,0 });
	bg->Init("menu_1");
	bg->AttachTo(this);

	bg = GameplayStatics::CreateObject<Bg>({ 150,25 });
	bg->Init("menu_2", 6);
	bg->AttachTo(this);
}
