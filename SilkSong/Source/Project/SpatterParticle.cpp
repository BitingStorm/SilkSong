#include "SpatterParticle.h"
#include "Components/ParticleSystem.h"


SpatterParticle::SpatterParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	SetRootComponent(particle);
	particle->SetCapacity(20);
	particle->SetInterval(0.01);
	particle->Load("particle_spatter");
	particle->SetMaxSpeed(8);
	particle->SetMinSpeed(0);
	particle->SetPattern(EParticlePattern::Line);
	particle->SetFadingType(EParticleFadingType::ExpandAndShrink);
	particle->SetFadingInTime(0.25f);
	particle->SetFadingOutTime(0.5f);
	particle->SetLifeCycle(0.75f);
	particle->SetSizeRange(0.2f, 0.6f);
	particle->SetIsLoop(false);

	DestroyTimerHandle.Bind(1.2f, [this]() {Destroy(); }, false);
}

void SpatterParticle::Init(FVector2D start, FVector2D end)
{
	SetLocalPosition((start + end) * 0.5f);
	FVector2D normal = end - start;
	particle->SetLine(FVector2D::Distance(start, end), FVector2D::VectorToDegree(FVector2D(-normal.y, normal.x)), 18.f - FMath::Clamp(int32(normal.Size() / 50), 0, 10), true);
	particle->SetCapacity(20 + int32(normal.Size() / 50));
	DestroyTimerHandle.SetDelay(1.2f + int32(normal.Size() / 50));
}