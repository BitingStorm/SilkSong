#include "Interactive.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"
#include "AttackBox.h"
#include "DamageResponseComponent.h"
#include "DamageStrategy.h"
#include "Effect.h"
#include "FragmentParticle.h"


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

void Interactive::Init_(std::string path, FVector2D boxSize, bool flag)
{
	path_ = path;
	box->SetSize(boxSize);
	this->flag = flag;
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
	flag ? GameModeHelper::PlayFXSound("sound_blockhit") : GameModeHelper::PlayFXSound("sound_swordhit");
	if(flag) GameplayStatics::CreateObject<FragmentParticle>(GetWorldPosition());
	GameplayStatics::PlayCameraShake(5);
	box->SetCollisonMode(CollisionMode::None);

	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	if (effect)
	{
		effect->SetLocalRotation(FMath::RandInt(-50, 0));
		effect->Init("effect_nailhit", -0.03f);
		effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.f));
	}
}
