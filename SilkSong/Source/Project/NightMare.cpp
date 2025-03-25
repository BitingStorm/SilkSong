#include "NightMare.h"
#include "Geo.h"
#include "GameplayStatics.h"
#include "PropertyComponent.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Components/SpriteRenderer.h"
#include "Player.h"
#include "Spike.h"
#include "GameModeHelper.h"
#include "RoarEffect.h"
#include "GrimmSmoke.h"
#include "FireBat.h"
#include "FireBall.h"
#include "SkyFire.h"
#include "Effect.h"
#include "Gate.h"
#include "Components/Camera.h"
#include "SmokeParticle.h"


NightMare::NightMare()
{
	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetSize({ 40,220 });
	box->SetLocalPosition({ 10,50 });
	box->SetType(CollisionType::Enemy);
	box->SetCollisonMode(CollisionMode::Collision);
	circle->SetLocalPosition({ 0,-100 });
	rigid->SetGravityEnabled(false);
	rigid->SetMass(10000);
	rigid->SetLinearDrag(0);

	idle.Load("nightmare_idle");
	idle.SetInterval(0.08f);
	bow.Load("nightmare_bow");
	bow.SetInterval(0.08f);
	bow.SetLooping(false);
	startteleport.Load("nightmare_teleport");
	startteleport.SetInterval(0.06f);
	startteleport.OnAnimEnter.Bind([this]() {
		circle->SetCollisonMode(CollisionMode::None); box->SetCollisonMode(CollisionMode::None); box->SetSize({ 40,220 });
		GameModeHelper::PlayFXSound("sound_nightmare_teleportin"); GameplayStatics::PlayCameraShake(5, 5);
		GameplayStatics::CreateObject<Effect>(GetWorldPosition())->Init("effect_pillar", -0.02f);
		});
	startteleport.OnAnimExit.Bind([this](){
		if (!player)return; 
		//¼¤Å­
		if ((property->GetMaxHealth() - property->GetHealth() > (balloonTimer + 1) * 63 + 9) && !ani->IsPlaying("fly"))
		{
			balloonTimer++;
			behaviorFlag = 5;
			BehaviorTimerHandle.Reset();
		}
		GameplayStatics::CreateObject<GrimmSmoke>(GetWorldPosition() + FVector2D(0.f, 100.f)); Move();
		});
	endteleport.Load("nightmare_teleport");
	endteleport.SetInterval(0.06f);
	endteleport.OnAnimEnter.Bind([this]() {
		ani->Deactivate(); render->Deactivate();
		TeleportTimerHandle.Bind(0.5f, [this]() {
			ani->Activate(); render->Activate(); GameplayStatics::PlayCameraShake(5, 5);
			GameplayStatics::CreateObject<GrimmSmoke>(GetWorldPosition() + FVector2D(0.f, 100.f));
			GameplayStatics::CreateObject<Effect>(GetWorldPosition())->Init("effect_pillar", -0.02f);
			});
		GameModeHelper::PlayFXSound("sound_nightmare_teleportout"); 
		});
	endteleport.OnAnimExit.Bind([this]() { 
		circle->SetCollisonMode(CollisionMode::Collision); box->SetCollisonMode(CollisionMode::Collision); Behave(); 
		});
	endteleport.SetReverse(true);
	startspike.Load("nightmare_startspike");
	startspike.SetInterval(0.08f);
	spike.Load("nightmare_spike");
	spike.SetInterval(0.08f);
	startballoon.Load("nightmare_startballoon");
	startballoon.SetInterval(0.1f);
	balloon.Load("nightmare_balloon");
	balloon.SetInterval(0.08f);
	balloon.OnAnimEnter.Bind([this]() {
		GameModeHelper::GetInstance()->GetAudioPlayer(1)->Play("sound_nightmare_fireballs", true);
		});
	balloon.OnAnimExit.Bind([this]() {
		GameModeHelper::GetInstance()->GetAudioPlayer(1)->Stop("sound_nightmare_fireballs");
		BalloonTimerHandle.Stop();
		});
	spawnBall.Bind(this, &NightMare::SpawnBall);
	balloon.AddNotification(0, spawnBall);
	cast.Load("nightmare_cast");
	cast.SetInterval(0.08f);
	cast.SetLooping(false);
	startairdash.Load("nightmare_startairdash");
	startairdash.SetInterval(0.08f);
	airdash.Load("nightmare_airdash");
	airdash.SetInterval(0.08f);
	airdash.OnAnimEnter.Bind([this]() {
		rigid->AddImpulse(FVector2D(-GetWorldScale().x, 1) * 10000 * 2000);
		box->SetSize({ 40,120 }); SetLocalRotation(-45.f);
		GameplayStatics::CreateObject<Effect>(GetWorldPosition(), GetWorldScale().x < 0 ? -45.f : -135.f)->Init("effect_dash");
		GameModeHelper::PlayFXSound("sound_nightmare_airdash");
		});
	airdash.OnAnimExit.Bind([this]() { SetLocalRotation(0); rigid->SetVelocity({}); });
	startdash.Load("nightmare_startdash");
	startdash.SetInterval(0.08f);
	dash.Load("nightmare_dash");
	dash.SetInterval(0.08f);
	dash.SetLooping(false);
	dash.OnAnimEnter.Bind([this]() {
		rigid->AddImpulse(FVector2D(-GetWorldScale().x * 0.75f, 0) * 10000 * 2000);
		GameplayStatics::CreateObject<Effect>(GetWorldPosition() - FVector2D(0, 25), 0, GetWorldScale())->Init("effect_dash_", -0.02f);
		GameModeHelper::PlayFXSound("sound_nightmare_dash");
		});
	dash.OnAnimExit.Bind([this]() {rigid->SetVelocity({});});
	startslash.Load("nightmare_startslash");
	startslash.SetInterval(0.08f);
	slash.Load("nightmare_slash");
	slash.SetInterval(0.08f);
	slash.OnAnimEnter.Bind([this]() {
		rigid->AddImpulse(FVector2D(-GetWorldScale().x, 1) * 10000 * 1250);	GameModeHelper::PlayFXSound("sound_sword_0");
		});
	slash.OnAnimExit.Bind([this]() {rigid->SetVelocity({}); });
	startuppercut.Load("nightmare_startuppercut");
	startuppercut.SetInterval(0.08f);
	uppercut.Load("nightmare_uppercut");
	uppercut.SetInterval(0.08f);
	uppercut.SetLooping(false);
	uppercut.OnAnimEnter.Bind([this]() {
		rigid->AddImpulse(FVector2D(-GetWorldScale().x * 0.5f, -1) * 10000 * 2000);
		GameModeHelper::PlayFXSound("sound_nightmare_uppercut"); GameModeHelper::PlayFXSound("sound_sword_1");
		});
	uppercut.OnAnimExit.Bind([this]() {rigid->SetVelocity({}); });
	stun.Load("nightmare_stun");
	stun.SetInterval(0.08f);
	fly.Load("nightmare_fly");
	fly.SetInterval(0.08f);
	die.Load("nightmare_die");
	die.SetInterval(0.08f);
	dieShake.Bind([]() {GameplayStatics::PlayCameraShake(4, 3); });
	die.AddNotification(1, dieShake);


	startteleport_to_endteleport.Init(startteleport, endteleport);
	endteleport_to_idle.Init(endteleport, idle);
	startspike_to_spike.Init(startspike, spike);
	startballoon_to_balloon.Init(startballoon, balloon);
	startairdash_to_airdash.Init(startairdash, airdash);
	startdash_to_dash.Init(startdash, dash);
	startslash_to_slash.Init(startslash, slash);
	slash_to_startuppercut.Init(slash, startuppercut);
	startuppercut_to_uppercut.Init(startuppercut, uppercut);
	stun_to_fly.Init(stun, fly);

	ani->Insert("idle", idle);
	ani->Insert("bow", bow);
	ani->Insert("startteleport", startteleport);
	ani->Insert("endteleport", endteleport);
	ani->Insert("startspike", startspike);
	ani->Insert("spike", spike);
	ani->Insert("startballoon", startballoon);
	ani->Insert("balloon", balloon);
	ani->Insert("cast", cast);
	ani->Insert("startairdash", startairdash);
	ani->Insert("airdash", airdash);
	ani->Insert("startdash", startdash);
	ani->Insert("dash", dash);
	ani->Insert("startslash", startslash);
	ani->Insert("slash", slash);
	ani->Insert("startuppercut", startuppercut);
	ani->Insert("uppercut", uppercut);
	ani->Insert("stun", stun);
	ani->Insert("fly", fly);
	ani->Insert("die", die);
	ani->SetNode("endteleport");

	render_death->SetTransparency(100);
}

void NightMare::BeginPlay()
{
	Super::BeginPlay();

	property->SetMaxHealth(300);
	property->AddHealth(300);

	box->OnComponentBeginOverlap.AddDynamic(Cast<Enemy>(this), &Enemy::OnOverlap);
	box->OnComponentHit.AddDynamic(this, &NightMare::OnHit);
	circle->OnComponentHit.AddDynamic(this, &NightMare::OnHit);

	BowTimerHandle.Bind(3.f, [this]() {
		ani->SetNode("bow");
		behaviorFlag = FMath::RandInt(1, 4);
		GameModeHelper::PlayBGMusic("nightmare");
		if (player)
		{
			player->Scare(false);
			player->EnableInput(true);
		}
		});

	BehaviorTimerHandle.Bind(4.f, [this]() {
		ani->SetNode("startteleport");
		behaviorFlag = FMath::RandInt(1, 4);
		}, true, 5.5f);

	CastTimerHandle.Bind(0.3f, [this]() {
		if (++castTimer > 6)return;
		if(castTimer == 3 || castTimer == 5 || castTimer == 6)
		GameplayStatics::CreateObject<FireBat>(GetWorldPosition() + FVector2D(0, FMath::RandReal(0, 50)), 0, FVector2D(-GetWorldScale().x, 1));
		}, true);
	castTimer = 6;


	BalloonTimerHandle.Bind(0.01f, [this]() {
		FVector2D unit = FVector2D::DegreeToVector(FMath::RandReal(0, 360));
		GameplayStatics::CreateObject<Effect>(GetWorldPosition() + unit * 120, 0, 
			FVector2D::UnitVector * FMath::RandReal(1.5f, 3.f))->Init("effect_flame_particle", 0.f, unit * FMath::RandReal(150, 200));
		}, true);
	BalloonTimerHandle.Stop();

	if (player)
	{
		player->Scare(true, "scare");
		player->EnableInput(false);
	}

	stunTimer = 0;
	balloonTimer = 0;
}

void NightMare::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (ani->IsPlaying("fly"))
	{
		rigid->SetVelocity(rigid->GetVelocity() * 0.98f);

		if (rigid->GetVelocity().Size() < 50)
		{
			if (FVector2D::Distance(FVector2D(-1000, 600), GetWorldPosition()) > 200)
			{
				rigid->AddImpulse((FVector2D(-1000, 600) - GetWorldPosition()).GetSafeNormal() * 10000 * 500);
			}
			else
			{
				rigid->AddImpulse(FVector2D::DegreeToVector(FMath::RandReal(0, 360)) * 10000 * 500);
			}
		}
	}
}

void NightMare::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	Super::ExecuteDamageTakenEvent(extraInfo);

	if (!extraInfo.bIsValid)
	{
		return;
	}

	//´òÔÎ
	if ((property->GetMaxHealth() - property->GetHealth()) > ((stunTimer + 1) * 63))
	{
		stunTimer++;
		ani->PlayMontage("stun");
		GameModeHelper::PlayFXSound("sound_nightmare_explode");
		GameModeHelper::PlayFXSound("sound_boss_stun");
		GameModeHelper::GetInstance()->GetAudioPlayer(1)->Play("sound_nightmare_circling", true);
		GameplayStatics::Pause(0.2f);
		BehaviorTimerHandle.Stop();
		RecoverTimerHandle.Bind(5.f, [this]() {
		    BehaviorTimerHandle.Continue(); BehaviorTimerHandle.Reset();
			GameModeHelper::PlayFXSound("sound_nightmare_appear");
			GameModeHelper::PlayFXSound("sound_nightmare_reform");
			GameModeHelper::GetInstance()->GetAudioPlayer(1)->Stop("sound_nightmare_circling");
			box->SetCollisonMode(CollisionMode::Collision);
			circle->SetLocalPosition({ 0,-100 });
			rigid->SetVelocity({});
			ani->SetNode("startteleport");
			render_death->Deactivate();
			});
		GameplayStatics::PlayCameraShake(7, 5);
		rigid->AddImpulse((FVector2D(-1000, 600) - GetWorldPosition()).GetSafeNormal() * 10000 * 500);
		box->SetCollisonMode(CollisionMode::None);
		circle->SetLocalPosition({ 0,0 });
		render_death->Activate();
	}
}

void NightMare::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (otherComp->GetType() == CollisionType::Block)
	{
		if (normalImpulse.y < 0 && ani->IsPlaying("airdash"))
		{
			GameplayStatics::PlayCameraShake(8, 4);
			ani->PlayMontage("startdash");
			GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D(0, 25), -90, FVector2D(1.75f, 0.5f))->Init("effect_dash", -0.02f);
		}
		else if (normalImpulse.y > 0 && ani->IsPlaying("uppercut"))
		{
			render->Deactivate();
			GameplayStatics::PlayCameraShake(10, 5);
			for (int i = 0; i < 6; i++)
			{
				GameplayStatics::CreateObject<SkyFire>(GetWorldPosition())->Init(FVector2D(-2.5f + i, 1.5f) * 400);
			}
			GameModeHelper::PlayFXSound("sound_skyflame");
		}
	}
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
	bIsDead = true;
	render->SetLayer(-1);
	GameplayStatics::PlayCameraShake(8, 15);

	render_death->SetTransparency(255);
	render_death->Activate();
	circle->OnComponentBeginOverlap.RemoveDynamic(Cast<Enemy>(this), &Enemy::OnOverlap);
	box->OnComponentBeginOverlap.RemoveDynamic(Cast<Enemy>(this), &Enemy::OnOverlap);

	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	if (effect)effect->Init("effect_death");

	BehaviorTimerHandle.Stop();
	ani->SetNode("die");
	GameModeHelper::PlayFXSound("sound_nightmare_defeat");
	GameModeHelper::PlayFXSound("sound_boss_finalhit");
	GameplayStatics::CreateObject<SmokeParticle>(GetWorldPosition());
	GameModeHelper::GetInstance()->GetAudioPlayer(0)->Stop("nightmare");

	DieTimerHandle.Bind(5.5f, [this]() {
		ani->SetNode("startteleport"); behaviorFlag = 6;
		rigid->SetMoveable(false); SpawnGeos();
		});
}

void NightMare::Move()
{
	FVector2D aim;
	int32 direction = (player->GetWorldPosition().x - GetWorldPosition().x) > 0 ? 1 : -1;
	switch (behaviorFlag)
	{
	case 0: break;
	case 1: aim = FVector2D(FMath::RandReal(-1400, -600), 850); break;
	case 2: aim = FVector2D(-350 * direction - 950, 850); break;
	case 3: aim = FVector2D(-500 * direction - 950, 850); break;
	case 4: aim = FVector2D(-400 * direction - 950, 400); break;
	case 5: aim = FVector2D(-1000, 600); break;
	case 6: aim = FVector2D(-1250, 850); break;
	default:break;
	}
	aim = ClampPosX(aim);
	SetLocalPosition(aim);
	SetLocalScale(FVector2D(-direction, 1));
}

void NightMare::Behave()
{
	if (behaviorFlag == 1)
	{
		ani->PlayMontage("startspike");
		GameModeHelper::PlayFXSound("sound_nightmare_cast_2");
		GameModeHelper::PlayFXSound("sound_spike_grounded");
		for (int i = 0; i < 10; i++)
		{
			GameplayStatics::CreateObject<Spike>(FVector2D(-1725 + i * 150, 700), FMath::RandInt(-3, 3));
		}
		BehaviorTimerHandle.SetDelay(3.8f);
	}
	else if (behaviorFlag == 2)
	{
		ani->PlayMontage("startslash");
		BehaviorTimerHandle.SetDelay(4.5f);
	}
	else if (behaviorFlag == 3)
	{
		ani->SetNode("cast");
		GameModeHelper::PlayFXSound("sound_nightmare_cast_1");
		BehaviorTimerHandle.SetDelay(4.2f);
		castTimer = 0;
		CastTimerHandle.Reset();
	}
	else if (behaviorFlag == 4)
	{
		ani->PlayMontage("startairdash");
		BehaviorTimerHandle.SetDelay(3.f);
	}
	else if (behaviorFlag == 5)
	{
		ani->PlayMontage("startballoon");
		GameModeHelper::PlayFXSound("sound_nightmare_cast_2");
		BehaviorTimerHandle.SetDelay(7.f);
		GameplayStatics::CreateObject<RoarEffect>(GetWorldPosition())->SetWhite();
		BalloonTimerHandle.Continue();
	}
	else if (behaviorFlag == 6)
	{
		ani->SetNode("idle");
		GameModeHelper::GetInstance()->GetAudioPlayer(0)->Play("bossdefeat_");
		GameplayStatics::FindObjectOfClass<Gate>()->Open();
		player->GetComponentByClass<Camera>()->SetRectFrame(FRect({ -1000.f,0.f }, { 1000.f,700.f }));
		circle->SetCollisonMode(CollisionMode::None);
		box->SetCollisonMode(CollisionMode::None);
	}
}

void NightMare::SpawnBall()
{
	balloonSpawnFlag = (balloonSpawnFlag + 1) % 3;
	float speed = FMath::RandReal(200, 300);
	if (balloonSpawnFlag == 0)
	{
		FVector2D unit = FVector2D::DegreeToVector(65);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		unit = FVector2D(unit.x, -unit.y);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		unit = FVector2D(-unit.x, unit.y);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		unit = FVector2D(unit.x, -unit.y);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
	}
	else if (balloonSpawnFlag == 1)
	{
		FVector2D unit = FVector2D::DegreeToVector(15);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		unit = FVector2D(-unit.x, unit.y);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		unit = FVector2D::DegreeToVector(270);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * 400);
	}
	else if (balloonSpawnFlag == 2)
	{
		FVector2D unit = FVector2D::DegreeToVector(-5);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		unit = FVector2D(-unit.x, unit.y);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * speed);
		if (FMath::RandPerc() > 0.5f)return;
		unit = FVector2D::DegreeToVector(-89);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * 400);
		unit = FVector2D(-unit.x, unit.y);
		GameplayStatics::CreateObject<FireBall>(GetWorldPosition() + unit * 120)->Init(unit * 400);
	}
}

FVector2D NightMare::ClampPosX(FVector2D pos)
{
	pos.x = FMath::Clamp(pos.x, -1550.f, -400.f);
	return pos;
}
