#include "LeafParticle.h"
#include "Components/ParticleSystem.h"

LeafParticle::LeafParticle()
{
	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->AddPosition({ 0, 300.f });
	particle->SetCapacity(20);
	particle->SetInterval(0.01f);
	particle->Load("particle_leaf");
	particle->SetMaxSpeed(125);
	particle->SetMinSpeed(75);
	particle->SetAngularSpeed(100.f, 150.f);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 250,350 }, { 80, 100 });
	particle->SetFadingOutTime(0.5f);
	particle->SetGravity(147.f);
	particle->SetLifeCycle(1.2f);
	particle->SetSizeRange(0.4f, 0.8f);
	particle->SetIsLoop(false);
	particle->SetLayer(5);

	DestroyTimerHandle.Bind(2.f, [this]() {Destroy(); }, false);
}
