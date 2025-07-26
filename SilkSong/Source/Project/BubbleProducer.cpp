#include "BubbleProducer.h"
#include "Components/ParticleSystem.h"

BubbleProducer::BubbleProducer()
{
	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetCapacity(15);
	particle->SetInterval(0.2f);
	particle->Load("particle_bubble");
	particle->SetMaxSpeed(60);
	particle->SetMinSpeed(20);
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(1200, 90, 25.f);
	particle->SetLayer(3);
	particle->SetFadingType(EParticleFadingType::ExpandAndShrink);
	particle->SetFadingInTime(0.4f);
	particle->SetFadingOutTime(0.6f);
	particle->SetTransparency(175);
	particle->SetLifeCycle(3.f);
}
