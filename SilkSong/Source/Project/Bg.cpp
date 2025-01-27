#include "Bg.h"
#include "Components/SpriteRenderer.h"
#include "GameplayStatics.h"
#include "Player.h"


Bg::Bg()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	bAdjust = true;
}

void Bg::Update(float deltaTime)
{
	Actor::Update(deltaTime);
	if (!bAdjust)return;

	if (player)
	{
		if ((player->GetCameraPos() - InitPos).Size() < 0.5f)return;
		SetLocalPosition(player->GetCameraPos() - (player->GetCameraPos() - InitPos) * pow(1.05, render->GetLayer()));
	}
	else
	{
		player = GameplayStatics::FindObjectOfClass<Player>();
	}
}

void Bg::Init(std::string path, int layer, bool blur, bool adjust)
{
	bAdjust = adjust;
	render->LoadSprite(path);
	render->SetLayer(layer);
	InitPos = GetWorldPosition();
	if (blur && layer < 0)
	{
		render->GaussianBlur(-layer + 2);
	}
}
