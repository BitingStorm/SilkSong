#include "RockParticle.h"
#include "Components/ParticleSystem.h"


RockParticle::RockParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(15);
	particle->SetInterval(0);
	particle->Load("particle_rock");
	particle->SetMaxSpeed(500);
	particle->SetMinSpeed(300);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 30,150 }, {30,150});
	particle->SetFadingOutTime(0.5f);
	particle->SetGravity(980.f);
	particle->SetLifeCycle(2.5f);
	particle->SetIsLoop(false);
	particle->SetLayer(3);

	DestroyTimerHandle.Bind(2.5f, [this]() {Destroy(); }, false);
}
