#include "Animated.h"
#include "Components/SpriteRenderer.h"



Animated::Animated()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
}

void Animated::Init(std::string name, float delay, int layer, BYTE alpha)
{
	idle.Load(name);
	idle.SetIndex(FMath::RandInt(0, 9));
	idle.SetInterval(delay);

	ani->Insert("idle", idle);
	ani->SetNode("idle");

	render->SetLayer(layer);
	render->SetLocalScale(FVector2D(1, 1) + FVector2D(0.05, 0) * layer);
	render->SetTransparency(alpha);
}
