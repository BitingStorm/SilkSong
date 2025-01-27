#include "Animated.h"
#include "Components/SpriteRenderer.h"
#include "Tools/Math.h"


Animated::Animated()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
}

void Animated::Init(std::string name, float delay, int layer)
{
	idle.Load(name);
	idle.SetIndex(Math::RandInt(0,9));
	idle.SetInterval(delay);

	ani->Insert("idle", idle);
	ani->SetNode("idle");

	render->SetLayer(layer);
	render->SetLocalScale(Vector2D(1, 1) + Vector2D(0.05, 0)*layer);
}
