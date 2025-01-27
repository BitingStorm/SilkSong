#include "Pointer.h"
#include "Components/SpriteRenderer.h"


Pointer::Pointer()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
	render->SetLayer(10);

	effect.Load("pointer");
	effect.SetInterval(0.03f);
	effect.SetLooping(false);

	destroy.Bind([this]() {Destroy(); });
	effect.AddNotification(0, destroy);

	ani->Insert("idle", effect);
	ani->SetNode("idle");
}

void Pointer::FadeOut()
{
	ani->SetReverse(true);
	effect.SetLooping(true);
}