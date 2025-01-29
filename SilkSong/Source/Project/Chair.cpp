#include "Chair.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"


Chair::Chair()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("chair");
	render->SetLayer(0);

	pointer = ConstructComponent<SpriteRenderer>();
	pointer->AttachTo(root);
	pointer->LoadSprite("pointer_sit");
	pointer->SetLayer(8);
	pointer->SetLocalPosition({0,-150});

	circle = ConstructComponent<CircleCollider>();
	circle->SetCollisonMode(CollisionMode::Trigger);
	circle->SetType(CollisionType::Item);
	circle->AttachTo(root);
	circle->SetRadius(75);
	circle->OnComponentBeginOverlap.AddDynamic(this, &Chair::OnBeginOverlap);
	circle->OnComponentEndOverlap.AddDynamic(this, &Chair::OnEndOverlap);
}

void Chair::DisablePointer()
{
	pointer->Deactivate();
}


void Chair::OnBeginOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (otherComp->GetType() == CollisionType::HurtBox)
	{
		pointer->Activate();
	}
}

void Chair::OnEndOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (otherComp->GetType() == CollisionType::HurtBox)
	{
		pointer->Deactivate();
	}
}

void Chair::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	pointer->AddPosition({ 0,deltaTime * dir * 50 });

	if (std::abs(pointer->GetLocalPosition().y + 150) >= 10)
	{
		dir = -dir;
		pointer->AddPosition({ 0,2 * deltaTime * dir * 100 });
	}
}
