#include "Wave.h"
#include "Components/SpriteRenderer.h"
#include "Components/Collider.h"
#include "Player.h"
#include "GameModeHelper.h"


Wave::Wave()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->SetLayer(1);

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Bullet);

	box->OnComponentBeginOverlap.AddDynamic(this, &Wave::OnOverlap);
}

void Wave::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	AddPosition(FVector2D(GetWorldScale().x * (index + 12) * deltaTime * 60, 0));

	if (FMath::Abs(GetWorldPosition().x) > 1200)
	{
		Destroy();
	}
}

void Wave::Init(int32 index, FVector2D size)
{
	render->LoadSprite(std::string("wave") + std::to_string(index));
	this->index = index;
	box->SetSize(size);
}

void Wave::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, player, 1, EDamageType::Enemy);
	}
}