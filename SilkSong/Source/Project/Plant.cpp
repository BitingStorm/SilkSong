#include "Plant.h"
#include "Components/SpriteRenderer.h"
#include "GameplayStatics.h"
#include "Player.h"
#include "DamageResponseComponent.h"
#include "Components/Collider.h"
#include "DamageStrategy.h"
#include "GameModeHelper.h"
#include "Effect.h"
#include "LeafParticle.h"


Plant::Plant()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetCollisonMode(CollisionMode::None);
	box->SetType(CollisionType::Chest);
	box->OnComponentBeginOverlap.AddDynamic(this, &Plant::OnBeginOverlap);

	damageResponse = ConstructComponent<DamageResponseComponent>();
	damageResponse->SetStrategy(new DamageStrategy());
}

void Plant::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (!moveLayer)return;

	if (player)
	{
		if ((player->GetCameraPos() - InitPos).Size() < 0.5f)return;
		SetLocalPosition(player->GetCameraPos() - (player->GetCameraPos() - InitPos) * pow(1.05, moveLayer));
	}
	else
	{
		player = GameplayStatics::FindObjectOfClass<Player>();
	}
}

void Plant::Init(std::string aniPath, float delay, int layer, int moveLayer)
{
	idle.Load(aniPath);
	idle.SetIndex(FMath::RandInt(0, 23));
	idle.SetInterval(delay);
	this->delay = delay;
	ani->Insert("idle", idle);
	ani->SetNode("idle");

	render->SetLayer(layer);
	InitPos = GetWorldPosition();
	this->moveLayer = moveLayer;
}

void Plant::Init_(std::string deathPath, FVector2D boxSize)
{
	box->SetCollisonMode(CollisionMode::Trigger);
	this->death = deathPath;
	box->SetSize(boxSize);
}

void Plant::OnBeginOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Cast<Player>(otherActor))
	{
		idle.SetInterval(delay * 0.25f);
		ShakeTimerHandle.Bind(0.6f, [this]() {idle.SetInterval(delay); });
	}
}

FDamageCauseInfo Plant::TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
{
	return damageResponse->TakeDamage(damageCauser, baseValue, damageType);
}

void Plant::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	Actor* causer = Cast<Actor>(extraInfo.damageCauser);
	CHECK_PTR(causer)
	if (death != std::string())
	{
		render->LoadSprite(death);
		ani->Deactivate();
	}
	GameModeHelper::PlayFXSound("sound_grasscut_" + std::to_string(FMath::RandInt(0, 1)));
	GameplayStatics::CreateObject<LeafParticle>(GetWorldPosition());
	
	box->SetCollisonMode(CollisionMode::None);

	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	effect->SetLocalRotation(FMath::RandInt(-50, 0));
	effect->Init("effect_nailhit", -0.03f);
	effect->SetLocalScale(FVector2D::UnitVector * FMath::RandReal(0.5f, 1.f));
}
