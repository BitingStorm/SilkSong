#include "GrimmSmoke.h"
#include "Components/ParticleSystem.h"


GrimmSmoke::GrimmSmoke()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(15);
	particle->SetInterval(0.01);
	particle->Load("particle_grimm_smoke");
	particle->SetMaxSpeed(250);
	particle->SetMinSpeed(50);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 50,250 }, { 50,130 });
	particle->SetFadingInTime(0.1f);
	particle->SetFadingOutTime(0.4f);
	particle->SetGravity(-98.f);
	particle->SetLifeCycle(0.6f);
	particle->SetIsLoop(false);
	particle->SetLayer(3);

	DestroyTimerHandle.Bind(1.f, [this]() {Destroy(); }, false);
}