#include "Spike.h"
#include "Components/SpriteRenderer.h"
#include "Components/Collider.h"
#include "Player.h"
#include "GameModeHelper.h"
#include "GameplayStatics.h"


Spike::Spike()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->SetLayer(1);

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Bullet);
	box->SetSize({ 50,575 });
	box->SetCollisonMode(CollisionMode::None);

	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	idle.Load("spike_idle");
	idle.SetInterval(0.05f);
	idle.SetLooping(false);
	idle.OnAnimEnter.Bind([this]() {
		GameplayStatics::PlayCameraShake(7, 5);
		IdleTimerHandle.Bind(0.5f, [this]() {ani->SetNode("end"); });
		box->OnComponentBeginOverlap.AddDynamic(this, &Spike::OnOverlap);
		box->SetCollisonMode(CollisionMode::Trigger);
		GameModeHelper::PlayFXSound("sound_spike_shootup");
		});
	idle.OnAnimExit.Bind([this]() {
		box->OnComponentBeginOverlap.RemoveDynamic(this, &Spike::OnOverlap);
		box->SetCollisonMode(CollisionMode::None);
		GameModeHelper::PlayFXSound("sound_spike_shrivelback");
		});
	ready.Load("spike_ready", { 0,275.f });
	ready.SetInterval(0.05f);
	ready.SetLooping(false);
	start.Load("spike_start", { 0,167.f });
	start.SetInterval(0.04f);
	end.Load("spike_end", { 0,81.f });
	end.SetInterval(0.04f);
	destroy.Bind([this]() {Destroy(); });
	end.AddNotification(0, destroy);

	start_to_idle.Init(start, idle);

	ani->Insert("idle", idle);
	ani->Insert("ready", ready);
	ani->Insert("start", start);
	ani->Insert("end", end);
	ani->SetNode("ready");

	AttackTimerHandle.Bind(0.6f, [this]() {ani->SetNode("start"); });
}

void Spike::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void Spike::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Player* player = Cast<Player>(otherActor))
	{
		GameModeHelper::ApplyDamage(this, player, 1, EDamageType::Enemy);
	}
}