#include "Interactive.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"
#include "AttackBox.h"
#include "DamageResponseComponent.h"
#include "DamageStrategy.h"


Interactive::Interactive()
{
	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetLocalPosition({ -5,10 });
	box->SetCollisonMode(CollisionMode::Trigger);
	box->SetType(CollisionType::Chest); 

	damageResponse = ConstructComponent<DamageResponseComponent>();
	damageResponse->SetStrategy(new DamageStrategy());
}

void Interactive::Update(float deltaTime)
{
	Bg::Update(deltaTime);
}

void Interactive::Init_(std::string path, FVector2D boxSize)
{
	path_ = path;
	box->SetSize(boxSize);
}

FDamageCauseInfo Interactive::TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
{
	return damageResponse->TakeDamage(damageCauser, baseValue, damageType);
}

void Interactive::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	Actor* causer = Cast<Actor>(extraInfo.damageCauser);
	CHECK_PTR(causer)
	FVector2D normal = (GetWorldPosition() - causer->GetWorldPosition()).GetSafeNormal();
	if (path_ != std::string())
	{
		render->LoadSprite(path_);
		if (normal.x < 0)
		{
			render->SetLocalScale({ -1, 1 });
		}
	}
	GameModeHelper::PlayFXSound("sound_swordhit");
	GameplayStatics::PlayCameraShake(5);
	box->SetCollisonMode(CollisionMode::None);
}
