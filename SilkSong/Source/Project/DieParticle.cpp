#include "DieParticle.h"
#include "Components/ParticleSystem.h"



DieParticle::DieParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(50);
	particle->SetInterval(0.03f);
	particle->Load("particle_heal");
	particle->SetMaxSpeed(350);
	particle->SetMinSpeed(250);
	particle->SetPattern(ParticlePattern::Center);
	particle->SetCenter({ 40,80 });
	particle->SetFadingOutTime(0.5f);
	particle->SetGravity(-9.8f);
	particle->SetLifeCycle(1.5f);
	particle->SetSizeRange(0.4f, 0.8f);
	particle->SetLayer(5);

	DestroyTimerHandle.Bind(3.f, [this]() {Destroy();}, false);
}