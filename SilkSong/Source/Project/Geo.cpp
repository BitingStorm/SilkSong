 #include "Geo.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Components/RigidBody.h"
#include "Player.h"
#include "Effect.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"



Geo::Geo()
{
	price = 1;

	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("player_light");
	render->SetLayer(3);


	box = ConstructComponent<BoxCollider>();
	box->SetCollisonMode(CollisionMode::Collision);
	box->SetType(CollisionType::Item);
	box->SetPhysicsMaterial(FPhysicsMaterial(0.5f, 1.f));
	box->AttachTo(root);

	rigid = ConstructComponent<RigidBody>();

	box->OnComponentBeginOverlap.AddDynamic(this, &Geo::OnOverlap);
	box->OnComponentHit.AddDynamic(this, &Geo::OnHit);

	rotateDelta = 0.005f;
}

void Geo::Init(std::string name, int price)
{
	this->price = price;
	float half = price == 1 ? 15 : 20;
	box->SetSize({ half,half });
	render->LoadSprite(name);
	rigid->AddImpulse(FVector2D(FMath::RandInt(-250, 250), FMath::RandInt(-300, -450)));
	rotateDelta = FMath::RandReal(0.002, 0.008) * (rigid->GetVelocity().x > 0 ? 1 : -1);
}

void Geo::Update(float deltaTime)
{
	Actor::Update(deltaTime);

	if (FMath::Abs(GetLocalScale().x) < 0.2)SetLocalScale({ -GetLocalScale().x,1});
	if (FMath::Abs(GetLocalScale().x) > 1)rotateDelta = -rotateDelta;
	SetLocalScale(GetLocalScale() + FVector2D(rotateDelta,0));
}

void Geo::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		player->AddGeo(price);
		Destroy();
		Effect* effect = GameplayStatics::CreateObject<Effect>(FVector2D(0.f, 20.f));
		if (!effect)return;
		effect->Init("effect_geo", -0.05f);
		effect->SetLocalScale(FVector2D(1, 1) * FMath::RandReal(1.25f + FMath::Sqrt(float(price)) * 0.1f, 1.5f + FMath::Sqrt(float(price)) * 0.1f));
		effect->SetLocalRotation(FMath::RandReal(-20, 20));
		effect->AttachTo(player);
		GameModeHelper::PlayFXSound("sound_geo_" + std::to_string(FMath::RandInt(0, 1)));
	}
}

void Geo::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (normalImpulse.y < 0)
	{
		GameModeHelper::PlayFXSound("sound_geo_hitground_" + std::to_string(FMath::RandInt(0, 1)));
	}
}
