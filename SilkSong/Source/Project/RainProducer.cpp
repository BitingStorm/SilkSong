#include "RainProducer.h"
#include "Components/ParticleSystem.h"


RainProducer::RainProducer()
{
	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetCapacity(60);
	particle->SetInterval(0.008f);
	particle->Load("rain_bg");
	particle->SetMaxSpeed(3500);
	particle->SetMinSpeed(2500);
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(2250,-90);
	particle->SetLayer(-5);
	particle->SetLifeCycle(0.5f);
	particle->SetLocalScale({ 0.75f,0.5f });

	particle_ = ConstructComponent<ParticleSystem>();
	particle_->AttachTo(root);
	particle_->SetCapacity(10);
	particle_->SetInterval(0.04f);
	particle_->Load("rain_bg_");
	particle_->SetMaxSpeed(4000);
	particle_->SetMinSpeed(2000);
	particle_->SetPattern(EParticlePattern::Line);
	particle_->SetLine(2000, -90);
	particle_->SetLifeCycle(0.4f);
	particle_->SetLayer(5);
}



