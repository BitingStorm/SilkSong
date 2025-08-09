#include "FireWorm.h"
#include "Components/SpriteRenderer.h"


FireWorm::FireWorm()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);

	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	idle.Load("fireworm");
	idle.SetInterval(0.06f);

	ani->Insert("idle", idle);
	ani->SetNode("idle");

	MoveTimerHandle.Bind(2.f + FMath::RandReal(-0.2f, 0.2f), [this]()
		{
			targetPos = initPos + FVector2D::DegreeToVector((FMath::RandPerc() > 0.5 ? FMath::RandReal(135.f, 225.f) : FMath::RandReal(-45.f, 45.f)))
				* radius * FMath::RandReal(0.2f, 1.f);
			if (targetPos.x - GetWorldPosition().x > 0)
			{
				SetLocalScale({ -1.f,1.f });
			}
			else
			{
				SetLocalScale({ 1.f,1.f });
			}
		}, true, FMath::RandPerc());
}

void FireWorm::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	SetLocalPosition(FMath::Lerp(GetWorldPosition(), targetPos, 0.02f));
}

void FireWorm::Init(int layer)
{
	render->SetLayer(layer);
	render->SetLocalScale(FVector2D::UnitVector * (1 + 0.13f * layer));
	radius = 150.f + layer * 20.f;
	radius = FMath::Clamp(radius, 50.f, 150.f);
	initPos = GetWorldPosition();
}
