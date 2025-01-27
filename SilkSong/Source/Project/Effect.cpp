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
}

void Effect::Init(std::string path, float delta)
{
	effect.Load(path);
	effect.SetInterval(0.08f + delta);
	
	ani->Insert(path, effect);
	ani->SetNode(path);
}
