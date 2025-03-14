#include "SoulMaster.h"
#include "PropertyComponent.h"
#include "GameplayStatics.h"
#include "Components/RigidBody.h"
#include "Components/SpriteRenderer.h"
#include "Components/Collider.h"
#include "AttackBox.h"
#include "SilkParticle.h"
#include "Effect.h"
#include "Geo.h"
#include "GameModeHelper.h"
#include "SoulOrb.h"
#include "RoarEffect.h"


SoulMaster::SoulMaster()
{
	requake.Bind([this]() {
		if (FMath::RandInt(0, 100) > 30)return;
		if (player)SetLocalPosition(FVector2D(player->GetWorldPosition().x, 350));
		ani->PlayMontage("startquake");
		GameModeHelper::PlayFXSound("sound_soulmaster_cast_" + std::to_string(FMath::RandInt(0, 3))); 
		});

	roar.Load("soulmaster_summon");
	roar.SetInterval(0.1f);
	idle.Load("soulmaster_idle");
	idle.SetInterval(0.1f);
	turn.Load("soulmaster_turn");
	turn.SetInterval(0.1f);
	startteleport.Load("soulmaster_teleport");
	startteleport.SetInterval(0.09f);
	startteleport.OnAnimExit.Bind([this]() {if (!player)return; Move(); });
	endteleport.Load("soulmaster_teleport");
	endteleport.SetInterval(0.09f);
	endteleport.SetReverse(true);
	endteleport.OnAnimExit.Bind([this]() {Behave(); });
	startsummon.Load("soulmaster_startsummon");
	startsummon.SetInterval(0.1f);
	summon.Load("soulmaster_summon");
	summon.SetInterval(0.1f);
	endsummon.Load("soulmaster_startsummon");
	endsummon.SetInterval(0.1f);
	endsummon.SetReverse(true);
	startquake.Load("soulmaster_startquake");
	startquake.SetInterval(0.1f);
	startquake.AddNotification(7, requake);
	quake.Load("soulmaster_quake");
	quake.SetInterval(0.1f);
	quake.OnAnimEnter.Bind([this]() {moveSpeed = FVector2D(0, 1); });
	quake.OnAnimExit.Bind([this]() {moveSpeed = FVector2D::ZeroVector; });
	startdash.Load("soulmaster_startdash");
	startdash.SetInterval(0.1f);
	dash.Load("soulmaster_dash");
	dash.SetInterval(0.1f);
	dash.OnAnimEnter.Bind([this]() {moveSpeed = FVector2D(1, 0) * GetWorldScale().x; });
	dash.OnAnimExit.Bind([this]() {moveSpeed = FVector2D::ZeroVector; });
	startstun.Load("soulmaster_startstun");
	startstun.SetInterval(0.1f);
	stun.Load("soulmaster_stun");
	stun.SetInterval(0.1f);
	stun.OnAnimEnter.Bind([]() {GameModeHelper::PlayFXSound("stun"); });
	stun.OnAnimExit.Bind([]() {GameModeHelper::GetInstance()->GetAudioPlayer(1)->Stop("stun"); });
	stuntransition.Load("soulmaster_transition");
	stuntransition.SetInterval(0.1f);
	stuntransition_.Load("soulmaster_transition");
	stuntransition_.SetInterval(0.1f);
	stuntransition.SetReverse(true);
	

	turn_to_idle.Init(turn, idle);
	startteleport_to_endteleport.Init(startteleport, endteleport);
	endteleport_to_idle.Init(endteleport, idle);
	startsummon_to_summon.Init(startsummon, summon);
	summon_to_endsummon.Init(summon, endsummon);
	endsummon_to_idle.Init(endsummon, idle);
	startquake_to_quake.Init(startquake, quake);
	startdash_to_dash.Init(startdash, dash);
	startstun_to_stuntransition.Init(startstun, stuntransition);
	stuntransition_to_stun.Init(stuntransition, stun);
	stun_to_stuntransition_.Init(stun, stuntransition_);
	stun_to_stuntransition_.AddCondition(AnimTransition::Trigger{ "recover" });
	stuntransition__to_idle.Init(stuntransition_, idle);

	ani->Insert("roar", roar);
	ani->Insert("idle", idle);
	ani->Insert("turn", turn);
	ani->Insert("startteleport", startteleport);
	ani->Insert("endteleport", endteleport);
	ani->Insert("startsummon", startsummon);
	ani->Insert("summon", summon);
	ani->Insert("endsummon", endsummon);
	ani->Insert("startquake", startquake);
	ani->Insert("quake", quake);
	ani->Insert("startdash", startdash);
	ani->Insert("dash", dash);
	ani->Insert("startstun", startstun);
	ani->Insert("stun", stun);
	ani->Insert("stuntransition", stuntransition);
	ani->Insert("stuntransition_", stuntransition_);
	ani->SetNode("roar");
	
	ani->AddParamater("recover", ParamType::Trigger);

	rigid->SetGravityEnabled(false);
	rigid->SetMoveable(false);

	circle->SetRadius(90);

	roarTimer = 0.f;
}

void SoulMaster::BeginPlay()
{
	Super::BeginPlay();

	property->SetMaxHealth(231);
	property->AddHealth(231);

	BehaviorTimerHandle.Bind(4.f, [this]() {
		ani->PlayMontage("startteleport");
		behaviorFlag = FMath::RandInt(0, 3);
		}, true, 5.f);

	RoarTimerHandle.Bind(0.25f, [this]() {
		GameplayStatics::PlayCameraShake(7,5);
		GameplayStatics::CreateObject<RoarEffect>(GetWorldPosition());
		roarTimer++;
		if (roarTimer > 12)
		{
			RoarTimerHandle.Stop();
			ani->SetNode("idle");
		}
		}, true);

	circle->OnComponentHit.AddDynamic(this, &SoulMaster::OnHit);

	GameModeHelper::PlayFXSound("sound_soulmaster_roar");

	SetLocalScale({ -1,1 });
}

void SoulMaster::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (player)
	{
		if ((player->GetWorldPosition() - GetWorldPosition()).x * GetWorldScale().x < 0 && ani->IsPlaying("idle"))
		{
			SetLocalScale(FVector2D(-GetWorldScale().x, 1));
			ani->PlayMontage("turn");
		}
	}

	if (moveSpeed != FVector2D::ZeroVector)
	{
		AddPosition(moveSpeed * deltaTime * (moveSpeed.x == 0 ? 5000 : 750));
	}
}

void SoulMaster::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	Super::ExecuteDamageTakenEvent(extraInfo);

	if (!extraInfo.bIsValid)
	{
		return;
	}

	if (ani->IsPlaying("stun") && !IsDead())
	{
		ani->SetTrigger("recover"); 	
		BehaviorTimerHandle.Continue();
		RecoverTimerHandle.Stop();
	}

	if (property->GetHealth() % 60 == 0 && property->GetHealth() != 0)
	{
		ani->PlayMontage("startstun");
		GameModeHelper::PlayFXSound("startstun");
		BehaviorTimerHandle.Stop();
		RecoverTimerHandle.Continue();
		RecoverTimerHandle.Bind(4.f, [this]() {ani->SetTrigger("recover"); 	BehaviorTimerHandle.Continue(); });
	}
}

void SoulMaster::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (otherComp->GetType() == CollisionType::Block)
	{
		if (normalImpulse.y < 0)
		{
			GameplayStatics::PlayCameraShake(15, 5);
			GameplayStatics::CreateObject<Effect>(GetWorldPosition() - FVector2D(0, 80))->Init("effect_soulmaster_quake", 0.01f);
			GameplayStatics::CreateObject<Effect>(GetWorldPosition() - FVector2D(0, 300))->Init("effect_soulmaster_quake_", 0.01f);
		}
		ani->PlayMontage("startteleport");
		behaviorFlag = 0;
	}
}

void SoulMaster::SpawnGeos()
{
	for (int i = 0; i < 20; i++)
	{
		Geo* geo = GameplayStatics::CreateObject<Geo>(GetWorldPosition());
		switch (FMath::RandInt(0,2))
		{
		case 0:geo->Init("1geo", 1); break;
		case 1:geo->Init("5geo", 5); break;
		case 2:geo->Init("25geo", 25); break;
		default:break;
		}
	}
}

void SoulMaster::Die()
{
	Super::Die();

	BehaviorTimerHandle.Stop();
	GameModeHelper::GetInstance()->GetAudioPlayer(0)->Stop("tearcity_boss");
	GameModeHelper::PlayBGMusic_("tearcity_o");
	GameModeHelper::PlayBGMusic_("tearcity_i");
}

void SoulMaster::Move()
{
	switch (behaviorFlag)
	{
	case 0: SetLocalPosition(FVector2D(player->GetWorldPosition().x, 1100) - FVector2D::DegreeToVector(FMath::RandReal(30.f, 150.f)) * 500); break;
	case 1: break;
	case 2: SetLocalPosition(FVector2D(player->GetWorldPosition().x, 350)); break;
	case 3: SetLocalPosition(FVector2D(-800 * GetWorldScale().x, 850)); break;
	default:break;
	}
}

void SoulMaster::Behave()
{
	switch (behaviorFlag)
	{
	case 0: break;
	case 1: ani->PlayMontage("startsummon"); GameplayStatics::CreateObject<SoulOrb>(GetWorldPosition());
		GameModeHelper::PlayFXSound("sound_soulmaster_cast_" + std::to_string(FMath::RandInt(0, 3))); break;
	case 2: 
		ani->PlayMontage("startquake");
		GameModeHelper::PlayFXSound("sound_soulmaster_cast_" + std::to_string(FMath::RandInt(0, 3))); break;
	case 3:
		ani->PlayMontage("startdash");
		GameModeHelper::PlayFXSound("sound_soulmaster_cast_" + std::to_string(FMath::RandInt(0, 3))); break;
	default:break;
	}
}
