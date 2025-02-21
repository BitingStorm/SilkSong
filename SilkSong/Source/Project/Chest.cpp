#include "Chest.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Geo.h"
#include "GameplayStatics.h"
#include "RockParticle.h"
#include "DamageResponseComponent.h"
#include "PropertyComponent.h"
#include "GameModeHelper.h"


Chest::Chest()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("chest");
	render->SetLayer(0);


	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Chest);
	box->SetSize({100,140});

	damageResponse = ConstructComponent<DamageResponseComponent>();
	property = ConstructComponent<PropertyComponent>();
	
	property->SetMaxHealth(5);
	property->AddHealth(5);	
}

FDamageCauseInfo Chest::TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
{
	FDamageCauseInfo damageInfo = damageResponse->TakeDamage(damageCauser, baseValue, damageType);
	property->AddHealth(-damageInfo.realValue);
	return damageInfo;
}

void Chest::ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)
{
}

void Chest::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	int num = FMath::RandInt(5, 8);

	for (int i = 0; i < num; i++)
	{
		Geo* geo = GameplayStatics::CreateObject<Geo>(GetWorldPosition());
		if (FMath::RandInt(0, 10) > 2)geo->Init("1geo", 1);
		else if (FMath::RandInt(0, 10) > 2)geo->Init("5geo", 5);
		else geo->Init("25geo", 25);
	}
	GameplayStatics::PlayCameraShake(5);
	GameModeHelper::PlayFXSound("sound_blockhit");

	if (property->GetHealth() <= 0)
	{
		render->LoadSprite("chest_");
		render->SetLocalPosition({ 0,14 });
		box->SetCollisonMode(CollisionMode::None);
		GameplayStatics::PlayCameraShake(10);
		GameplayStatics::CreateObject<RockParticle>(GetWorldPosition());
	}
}