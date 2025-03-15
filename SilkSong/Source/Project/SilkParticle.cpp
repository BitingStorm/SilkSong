#include "SilkParticle.h"
#include "Components/ParticleSystem.h"



SilkParticle::SilkParticle()
{
	silk = ConstructComponent<ParticleSystem>();
	silk->AttachTo(root);
	silk->SetCapacity(25);
	silk->SetInterval(0.01);
	silk->Load("particle_silk");
	silk->SetMaxSpeed(500);
	silk->SetMinSpeed(100);
	silk->SetFadingOutTime(0.5f);
	silk->SetPattern(EParticlePattern::Center);
	silk->SetGravity(98);
	silk->SetIsLoop(false);
	silk->SetLifeCycle(1.f);
	silk->SetSizeRange(0.4f, 0.8f);

	DestroyTimerHandle.Bind(1.25f, [this]() {Destroy(); }, false);
}

void SilkParticle::Init(FVector2D normal, bool dead)
{
	if (dead)
	{
		silk->SetCenter({ 20,200 }, { 0,360 });
		silk->SetCapacity(35);
		silk->SetMaxSpeed(300);
		silk->SetInterval(0);
	}
	else
	{
		float degree = FVector2D::VectorToDegree(normal);
		silk->SetCenter({ 50,150 }, { degree - 20,degree + 20 });
	}
}