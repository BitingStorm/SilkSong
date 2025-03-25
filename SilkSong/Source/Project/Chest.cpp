#include "Chest.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Geo.h"
#include "GameplayStatics.h"
#include "RockParticle.h"
#include "DamageResponseComponent.h"
#include "PropertyComponent.h"
#include "GameModeHelper.h"
#include "Effect.h"
#include "Components/ParticleSystem.h"


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

	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetCapacity(8);
	particle->SetInterval(0.03f);
	particle->Load("particle_smoke");
	particle->SetMaxSpeed(175);
	particle->SetMinSpeed(75);
	particle->SetPattern(EParticlePattern::Center);
	particle->SetCenter({ 0,50 }, { -30,210 });
	particle->SetFadingInTime(0.25f);
	particle->SetFadingOutTime(0.25f);
	particle->SetLifeCycle(1.f);
	particle->SetGravity(-9.8f);
	particle->SetLayer(3);
	particle->Deactivate();

	damageResponse = ConstructComponent<DamageResponseComponent>();
	property = ConstructComponent<PropertyComponent>();
	
	property->SetMaxHealth(5);
	property->AddHealth(5);	
}

void Chest::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (smokeTimer > 0)
	{
		smokeTimer -= deltaTime;
		if (smokeTimer <= 0)
		{
			particle->SetIsLoop(false);
		}
	}

	if (intensity <= 0) 
	{
		intensity = 0;
		return; 
	}
	float radian = FMath::DegreeToRadian(FMath::RandReal(0, 360));
	SetLocalPosition(originPos + intensity * FVector2D(FMath::Cos(radian), FMath::Sin(radian)));
	intensity -= deltaTime * 12;
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
	GameModeHelper::PlayFXSound("sound_geo_rockhit_" + std::to_string(FMath::RandInt(0, 2)));

	if (property->GetHealth() <= 0)
	{
		render->LoadSprite("chest_");
		render->SetLocalPosition({ 0,14 });
		box->SetCollisonMode(CollisionMode::None);
		GameplayStatics::PlayCameraShake(10);
		GameplayStatics::CreateObject<RockParticle>(GetWorldPosition());
	}

	if (intensity <= 0)
	{
		originPos = GetWorldPosition();
		intensity = 4;
	}

	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	if (effect)
	{
		effect->SetLocalRotation(FMath::RandInt(-50, 0));
		effect->Init("effect_nailhit", -0.03f);
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.8, 1.2));
	}

	particle->Activate();
	particle->SetIsLoop(true);
	smokeTimer = 0.4f;

	Actor* causer = Cast<Actor>(extraInfo.damageCauser);
	CHECK_PTR(causer)
	FVector2D normal = (GetWorldPosition() - causer->GetWorldPosition()).GetSafeNormal();
	float degree = FVector2D::VectorToDegree(normal);
	particle->SetCenter({ 0,50 }, { degree - 90.f,degree + 90.f });
}

