#include "FragmentParticle.h"
#include "Components/ParticleSystem.h"


FragmentParticle::FragmentParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(12);
	particle->SetInterval(0);
	particle->Load("particle_fragment");
	particle->SetMaxSpeed(400);
	particle->SetMinSpeed(200);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 0,50 }, { 30,150 });
	particle->SetFadingOutTime(0.3f);
	particle->SetGravity(980.f);
	particle->SetLifeCycle(1.f);
	particle->SetIsLoop(false);
	particle->SetLayer(3);

	DestroyTimerHandle.Bind(1.f, [this]() {Destroy(); }, false);
}
