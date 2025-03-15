#include "SitParticle.h"
#include "Components/ParticleSystem.h"


SitParticle::SitParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(25);
	particle->SetInterval(0.01f);
	particle->Load("particle_heal");
	particle->SetMaxSpeed(150);
	particle->SetMinSpeed(75);
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(length = 75,90);
	particle->SetFadingOutTime(0.5f);
	particle->SetLifeCycle(1.5f);
	particle->SetSizeRange(0.25f, 0.45f);
	particle->SetIsLoop(false);
	particle->SetLayer(5);
	
	DestroyTimerHandle.Bind(1.75f, [this]() {Destroy(); }, false);
}

void SitParticle::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	particle->SetLine(length += deltaTime * 200, 90);
}