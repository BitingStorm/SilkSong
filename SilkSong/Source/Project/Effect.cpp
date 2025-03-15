#include "Effect.h"
#include "Components/SpriteRenderer.h"


Effect::Effect()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
	render->SetLayer(3);

	destroy.Bind([this]() {Destroy(); });
	effect.AddNotification(0, destroy);

	velocity = FVector2D::ZeroVector;
}

void Effect::Init(std::string path, float delta, FVector2D vel)
{
	effect.Load(path);
	effect.SetInterval(0.08f + delta);
	
	ani->Insert(path, effect);
	ani->SetNode(path);

	velocity = vel;
}

void Effect::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (velocity != FVector2D::ZeroVector)
	{
		velocity.y -= 400.f * deltaTime;
		AddPosition(velocity * deltaTime);
	}
}