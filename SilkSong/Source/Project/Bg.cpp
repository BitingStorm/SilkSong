#include "Bg.h"
#include "Components/SpriteRenderer.h"
#include "GameplayStatics.h"
#include "Player.h"


Bg::Bg()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	moveLayer = 0;
}

void Bg::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (!moveLayer)return;

	if (player)
	{
		if ((player->GetCameraPos() - InitPos).Size() < 0.5f)return;
		SetLocalPosition(player->GetCameraPos() - (player->GetCameraPos() - InitPos) * pow(k, moveLayer));
	}
	else
	{
		player = GameplayStatics::FindObjectOfClass<Player>();
	}
}

void Bg::Init(std::string path, int layer, int moveLayer, bool blur, float k)
{
	render->LoadSprite(path);
	render->SetLayer(layer);
	InitPos = GetWorldPosition();
	this->moveLayer = moveLayer;
	this->k = k;
	if (blur && layer < 0)
	{
		render->GaussianBlur(-layer + 1);
	}
}
