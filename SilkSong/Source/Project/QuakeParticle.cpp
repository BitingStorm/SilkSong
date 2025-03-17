#include "QuakeParticle.h"
#include "Components/ParticleSystem.h"


QuakeParticle::QuakeParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(40);
	particle->SetInterval(0);
	particle->Load("particle_quake");
	particle->SetMaxSpeed(1750);
	particle->SetMinSpeed(1000);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({200,250}, {80,100});
	particle->SetFadingType(EParticleFadingType::ExpandAndShrink);
	particle->SetFadingOutTime(0.3f);
	particle->SetGravity(1960.f);
	particle->SetLifeCycle(2.f);
	particle->SetIsLoop(false);
	particle->SetLayer(1);
	particle->SetSizeRange(0.4f, 1.f);

	DestroyTimerHandle.Bind(2.f, [this]() {Destroy(); }, false);
}