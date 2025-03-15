#include "Pointer.h"
#include "Components/SpriteRenderer.h"


Pointer::Pointer()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
	render->SetLayer(10);

	load.Load("pointer");
	load.SetInterval(0.03f);
	load.SetLooping(false);
	minus.Load("pointer");
	minus.SetInterval(0.03f);
	minus.SetReverse(true);

	destroy.Bind([this]() {Destroy(); });
	minus.AddNotification(0, destroy);

	ani->Insert("load", load);
	ani->Insert("minus", minus);
	ani->SetNode("load");
}

void Pointer::FadeOut()
{
	if(!ani->IsPlaying("minus"))ani->SetNode("minus");
}