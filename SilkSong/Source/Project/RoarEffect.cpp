#include "RoarEffect.h"
#include "Components/SpriteRenderer.h"


RoarEffect::RoarEffect()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	render->SetLayer(8); 
	render->LoadSprite("roar_effect");
	render->SetTransparency(200);
}

void RoarEffect::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	SetLocalScale(GetLocalScale() * 1.2f);

	if (GetLocalScale().x > 10)
	{
		Destroy();
	}
}

void RoarEffect::SetWhite()
{
	render->LoadSprite("roar_effect_white");
}