#include "Enemy.h"
#include "Components/SpriteRenderer.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Player.h"
#include "PlatForm.h"
#include "GameplayStatics.h"
#include "Effect.h"
#include "Components/AudioPlayer.h"
#include "Components/ParticleSystem.h"
#include "SilkParticle.h"


Enemy::Enemy()
{
	render = ConstructComponent<SpriteRenderer>();
	render->SetLayer(1);
	circle = ConstructComponent<CircleCollider>();
	rigid = ConstructComponent<RigidBody>();
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	audio = ConstructComponent<AudioPlayer>();
	audio->AttachTo(root);
	audio->SetSpacial(true);
	audio->SetRadius(500,1000);

	SetRootComponent(render);
	render_death = ConstructComponent<SpriteRenderer>();
	render_death->AttachTo(root);
	render_death->LoadSprite("death_light");
	render_death->SetLayer(-1);
	render_death->Deactivate();
	
	circle->AttachTo(root);
	circle->SetRadius(40);
	circle->SetType(CollisionType::Enemy);
	circle->SetCollisonMode(CollisionMode::Collision);
}

void Enemy::BeginPlay()
{
	Super::BeginPlay();

	player = GameplayStatics::FindObjectOfClass<Player>();

	circle->OnComponentBeginOverlap.AddDynamic(this, &Enemy::OnOverlap);
}

void Enemy::Update(float deltaTime)
{
    Super::Update(deltaTime);

	if (bIsDead && render_death->GetEnabled())
	{
		int trans = int(render_death->GetTransparency()) - int(deltaTime * 500);
		if (trans > 0) { render_death->SetTransparency(trans); render_death->SetLocalScale(GetLocalScale()*0.98f); }
		else render_death->Deactivate();
	}

	if (bIsDead && std::abs(rigid->GetVelocity().x) < 0.5f)
	{
		rigid->SetMoveable(false);
		rigid->SetRotatable(false);
		circle->SetCollisonMode(CollisionMode::None);
	}

	if (!bIsDead && GetWorldPosition().y > 1080) { blood -= 50; TakeDamage({ 0,0 },false); rigid->AddImpulse({0,-500}); }
}

void Enemy::TakeDamage(FVector2D normal, bool gain)
{
	rigid->AddImpulse({ normal.x * 600,-200 });
	GameplayStatics::PlayCameraShake(4);
	render->Blink(0.3f, WHITE, 100);
	blood -= 3;
	SilkParticle* silk = GameplayStatics::CreateObject<SilkParticle>();
	silk->AttachTo(this);
	silk->Init(normal, blood <= 0);

	if (!player)return;
	float delta_x = player->GetWorldPosition().x - GetWorldPosition().x;
	if (gain)player->AddSilk(1);


	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	if (effect)
	{
		effect->SetLocalRotation(FMath::RandInt(-15, 15) + FVector2D::VectorToDegree(normal)); effect->Init("effect_attack", -0.03f);
		effect->SetLocalScale(FVector2D{ delta_x < 0 ? 1.f : -1.f ,1.f }*FMath::RandReal(1, 1.5));
	}

	if (blood <= 0 && !bIsDead)
	{
		Die();
		rigid->SetAngularVelocity(100 * (delta_x > 0 ? 1.f : -1.f) * FMath::RandPerc());
		SpawnGeos();
	}
	else
	{
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D((delta_x < 0 ? 1.f : -1.f) * 300, 0));
		if (!effect)return;
		effect->Init("effect_attack_", 0.02f);
		effect->SetLocalScale(FVector2D{ delta_x < 0 ? 1.f : -1.f ,1.f }*FMath::RandReal(0.9, 1.3));
		effect->SetLocalRotation(FMath::RandInt(10, -10));
	}
}

void Enemy::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		FVector2D normal = (-GetWorldPosition() + player->GetWorldPosition()).GetSafeNormal();
		player->TakeDamage(normal);
	}
}

void Enemy::Die()
{
	bIsDead = true;
	render->SetLayer(-1);
	GameplayStatics::PlayCameraShake(8, 15);
	render_death->Activate();
	render_death->SetLocalPosition(GetWorldPosition());

	render_death->DetachFrom(root);
	render_death->SetOwner(nullptr);//开发者偷懒而使用的危险代码，请勿模仿
	
	circle->OnComponentBeginOverlap.RemoveDynamic(this, &Enemy::OnOverlap);
	circle->SetPhysicsMaterial(FPhysicsMaterial(0.6f,0.6f));
}
