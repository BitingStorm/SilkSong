#include "NightMare.h"
#include "Geo.h"
#include "GameplayStatics.h"
#include "PropertyComponent.h"


NightMare::NightMare()
{
	idle.Load("nightmare_idle");
	idle.SetInterval(0.1f);
	bow.Load("nightmare_bow");
	bow.SetInterval(0.1f);


	ani->Insert("idle", idle);
	ani->Insert("bow", bow);
	ani->SetNode("idle");
}

void NightMare::BeginPlay()
{
	Super::BeginPlay();

	property->SetMaxHealth(900);
	property->AddHealth(900);
}

void NightMare::Update(float deltaTime)
{
	Super::Update(deltaTime);
}

void NightMare::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	Super::ExecuteDamageTakenEvent(extraInfo);

	if (!extraInfo.bIsValid)
	{
		return;
	}
}

void NightMare::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
}

void NightMare::SpawnGeos()
{
	for (int i = 0; i < 15; i++)
	{
		Geo* geo = GameplayStatics::CreateObject<Geo>(GetWorldPosition());
		geo->Init("25geo", 25);
	}
}

void NightMare::Die()
{
	Super::Die();
}
