#include "OldTheme.h"
#include "GameplayStatics.h"
#include "Components/ParticleSystem.h"
#include "Bg.h"


OldTheme::OldTheme()
{
	blackRadiantFar = ConstructComponent<ParticleSystem>();
	blackRadiantFar->SetLocalPosition({ 0,250 });
	blackRadiantFar->AttachTo(root);
	blackRadiantFar->SetCapacity(20);
	blackRadiantFar->SetInterval(0.3f);
	blackRadiantFar->Load("menu_radiant_bottom");
	blackRadiantFar->SetMaxSpeed(30);
	blackRadiantFar->SetMinSpeed(10);
	blackRadiantFar->SetPattern(EParticlePattern::Line);
	blackRadiantFar->SetLine(1000, 90);
	blackRadiantFar->SetLayer(1);
	blackRadiantFar->SetLifeCycle(7.5f);
	blackRadiantFar->SetFadingOutTime(1.f);
	blackRadiantFar->SetFadingInTime(1.f);
	blackRadiantFar->SetFadingType(EParticleFadingType::ExpandAndShrink);
	blackRadiantFar->SetGravity(0);
	blackRadiantFar->SetSizeRange(0.35, 0.75);

	blackRadiantClose = ConstructComponent<ParticleSystem>();
	blackRadiantClose->SetLocalPosition({ 0,350 });
	blackRadiantClose->AttachTo(root);
	blackRadiantClose->SetCapacity(12);
	blackRadiantClose->SetInterval(0.4f);
	blackRadiantClose->Load("menu_radiant_bottom");
	blackRadiantClose->SetMaxSpeed(80);
	blackRadiantClose->SetMinSpeed(30);
	blackRadiantClose->SetPattern(EParticlePattern::Line);
	blackRadiantClose->SetLine(800, 90);
	blackRadiantClose->SetLayer(1);
	blackRadiantClose->SetLifeCycle(6.5f);
	blackRadiantClose->SetFadingOutTime(0.5f);
	blackRadiantClose->SetFadingInTime(0.5f);
	blackRadiantClose->SetFadingType(EParticleFadingType::ExpandAndShrink);
	blackRadiantClose->SetGravity(0);
	blackRadiantClose->SetSizeRange(0.75, 1.2);

	smoke = ConstructComponent<ParticleSystem>();
	smoke->SetLocalPosition({ 0,400 });
	smoke->AttachTo(root);
	smoke->SetCapacity(5);
	smoke->SetInterval(0.8f);
	smoke->Load("menu_smoke");
	smoke->SetMaxSpeed(50);
	smoke->SetMinSpeed(20);
	smoke->SetPattern(EParticlePattern::Line);
	smoke->SetLine(1000, 90);
	smoke->SetLayer(2);
	smoke->SetLifeCycle(4.f);
	smoke->SetFadingOutTime(1.f);
	smoke->SetFadingInTime(1.f);
	smoke->SetGravity(0);
	smoke->SetSizeRange(0.5, 1.5);

	Bg* bg = GameplayStatics::CreateObject<Bg>({ 0,-25 });
	bg->Init("menu_0");
	bg->AttachTo(this);
}
