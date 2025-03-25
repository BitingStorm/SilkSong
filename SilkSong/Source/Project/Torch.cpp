#include "Torch.h"
#include "Components/SpriteRenderer.h"
#include "Components/ParticleSystem.h"
#include "Animated.h"
#include "GameplayStatics.h"


Torch::Torch()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("grimmtent_torch");
	render->SetLayer(3);

	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetLocalPosition({ 0,90.f });
	particle->SetCapacity(15);
	particle->SetInterval(0.15f);
	particle->Load("particle_fire");
	particle->SetMaxSpeed(125);
	particle->SetMinSpeed(50);
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(50, 90, 25);
	particle->SetFadingOutTime(0.25f);
	particle->SetGravity(-98.f);
	particle->SetLifeCycle(2.25f);
	particle->SetLayer(4);

	Animated* anim = GameplayStatics::CreateObject<Animated>({ 0,90.f });
	anim->Init("flame_torch", 0.06f, 2);
	anim->AttachTo(this);

	anim = GameplayStatics::CreateObject<Animated>({ 0,90.f });
	anim->Init("glow_torch", 0.08f, 4);
	anim->AttachTo(this);
}
