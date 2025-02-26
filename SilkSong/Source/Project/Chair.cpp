#include "Chair.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "SceneUI.h"
#include "GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "Player.h"


Chair::Chair()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("chair");
	render->SetLayer(0);

	circle = ConstructComponent<CircleCollider>();
	circle->SetType(CollisionType::Item);
	circle->AttachTo(root);
	circle->SetRadius(75);
	circle->OnComponentBeginOverlap.AddDynamic(this, &Chair::OnBeginOverlap);
	circle->OnComponentEndOverlap.AddDynamic(this, &Chair::OnEndOverlap);


	widget = ConstructComponent<WidgetComponent>();
	widget->AttachTo(root);
	widget->AddPosition({0,-150});
	widget->SetUI(GameplayStatics::CreateUI<SceneUI>());
	widget->SetSize({100,100});
	widget->Deactivate();
}


void Chair::OnBeginOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Cast<Player>(otherActor))
	{
		widget->Activate();
	}
}

void Chair::OnEndOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Cast<Player>(otherActor))
	{
		widget->Deactivate();
	}
}

void Chair::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	widget->AddPosition({ 0,deltaTime * dir * 50 });

	if (FMath::Abs(widget->GetLocalPosition().y + 150) >= 8)
	{
		dir = -dir;
		widget->AddPosition({ 0,2 * deltaTime * dir * 100 });
	}
}
