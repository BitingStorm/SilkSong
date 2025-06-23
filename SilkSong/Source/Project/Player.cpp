#include "Player.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Components/RigidBody.h"
#include "Components/Camera.h"
#include "Components/ParticleSystem.h"
#include "DamageResponseComponent.h"
#include "DefaultDamageStrategy.h"
#include "PlayerPropertyComponent.h"
#include "GameplayStatics.h"
#include "GameModeHelper.h"

#include "PlayerAnimator.h"
#include "Effect.h"
#include "AttackBox.h"
#include "HealParticle.h"
#include "DieParticle.h"
#include "SitParticle.h"
#include "GameUI.h"
#include "CloseSkillBox.h"
#include "Dart.h"
#include "Needle.h"
#include "Chair.h"


Player::Player()
{
	render = GetComponentByClass<SpriteRenderer>();
	render->SetLayer(1);
	render_light = ConstructComponent<SpriteRenderer>();
	render_light->AttachTo(root);
	render_light->LoadSprite("player_light");
	render_light->SetLayer(0);
	render_light->SetTransparency(75);
	
	ani = ConstructComponent<PlayerAnimator>();
	ani->SetupAttachment(render);

	box = GetComponentByClass<BoxCollider>();
	box->SetSize({30,130});
	box->SetLocalPosition({-5,10});
	box->SetCollisonMode(CollisionMode::Collision);
	box->SetType(CollisionType::Player);
	box->SetPhysicsMaterial(FPhysicsMaterial(0.1f,0));

	hurtBox = ConstructComponent<BoxCollider>();
	hurtBox->AttachTo(root);
	hurtBox->SetSize({ 36,110 });
	hurtBox->SetLocalPosition({ -5,10 });
	hurtBox->SetType(CollisionType::HurtBox);

	rigid = GetComponentByClass<RigidBody>();
	rigid->SetLinearDrag(0.07f);
	rigid->SetGravity(1960);

	camera = GetComponentByClass<Camera>();
	camera->SetDistanceThreshold(100);
	camera->SetSmoothness(50);
	camera->SetRectFrame(FRect({ -100.f,-250.f }, { 100.f,750.f }));

	particle = ConstructComponent<ParticleSystem>();
	particle->AttachTo(root);
	particle->SetLocalPosition({ 0,50 });
	particle->SetCapacity(15);
	particle->SetInterval(0.1f);
	particle->Load("particle_heal");
	particle->SetMaxSpeed(100);
	particle->SetMinSpeed(50);
	particle->SetPattern(EParticlePattern::Line);
	particle->SetLine(150, 90);
	particle->SetFadingType(EParticleFadingType::ExpandAndShrink);
	particle->SetFadingInTime(0.5f);
	particle->SetFadingOutTime(0.5f);
	particle->SetGravity(-9.8f);
	particle->SetLifeCycle(1.5f);
	
	damageResponse = ConstructComponent<DamageResponseComponent>();
	playerProperty = ConstructComponent<PlayerPropertyComponent>();

	bGround = false;
	bDashing = false;
	bEvading = false;
	bFloating = false;
	bRushing = false;
	bRushFlag = false;
	bSitting = false;
	bWall = false;

	lastJumpTime = 0.f;
	lastAttackTime = 0.f;
	lastEvadeTime = 0.f;
	lastDashTime = 0.f;
	lastFloatTime = 0.f;
	lastThrowTime = 0.f;
	
	
	attackFlag = 0;
	lookFlag = 0;
	jumpFlag = 1;
	walkLock = 0;
	direction = ECharacterDirection::LookForward;


	ui = GameplayStatics::CreateUI<GameUI>();
	ui->AddToViewport();

	ani->dashEffect.Bind([this]()
		{
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() - FVector2D(GetWorldScale().x * 150, 0));
			if (bGround)
			{
				effect->Init("effect_dash"); effect->SetLocalScale(GetWorldScale());
			}
			else
			{
				effect->Init("effect_dash_", -0.03f); effect->SetLocalScale({ -GetWorldScale().x,1.f }); effect->AddPosition({ -GetWorldScale().x * 125,-50 });
			}
			bDashing = true;
		});
	ani->cureEffect.Bind([this]()
		{
			GameplayStatics::CreateObject<HealParticle>()->AttachTo(this);
			ui->WhiteBlink(6);
		});
	ani->wetWalkEffect.Bind([this]()
		{
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D(0, 60));
			effect->Init("effect_wetwalk");
			effect->SetLocalScale(GetWorldScale() * FMath::RandReal(0.8f, 1.1f));
		});
	ani->dartSpawn.Bind([this]()
		{
			Dart* dart = GameplayStatics::CreateObject<Dart>(GetWorldPosition());
			if (dart)dart->Init(GetWorldScale().x < 0);
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
			effect->Init("effect_throw", -0.01f); effect->SetLocalScale(GetLocalScale());
			effect->AddPosition({GetLocalScale().x * 50,0});
			AddDart(-1);
		});
	ani->needleSpawn.Bind([this]()
		{
			Needle* needle = GameplayStatics::CreateObject<Needle>({ -500,0 });
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
			needle->AttachTo(effect);
			effect->Init("effect_remoteskill"); effect->SetLocalScale(GetLocalScale());
			effect->AddPosition({ GetLocalScale().x * 500,25 });
			GameModeHelper::PlayFXSound("sound_remoteskill");
		});
	ani->needleSpawn_.Bind([this]()
		{
			Needle* needle = GameplayStatics::CreateObject<Needle>(GetWorldPosition() + FVector2D(0, 10));
			needle->Init(true);
			needle->SetLocalScale(GetWorldScale());
			Effect* effect = GameplayStatics::CreateObject<Effect>();
			effect->AttachTo(needle);
			effect->Init("effect_rapidskill", -0.05f);

			effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() - FVector2D(0, 20));
			effect->Init("effect_remoteskill", -0.06f); effect->SetLocalScale(FVector2D(GetLocalScale().x * 0.5f, 1));
			effect->AddPosition({ GetLocalScale().x * 150,25 });
			GameModeHelper::PlayFXSound("sound_hardattack");
		});
	ani->grabFinished.Bind(this, &Player::Grab);
	ani->downAttackSpawn.Bind([this]()
		{
			AttackBox* attackBox = GameplayStatics::CreateObject<AttackBox>();
			attackBox->AttachTo(this);
			attackBox->SetLocalPosition(FVector2D(70, 35));
			attackBox->Init(ECharacterDirection::LookDown);
		});
	ani->leaveStart.Bind([this]()
		{
			rigid->SetVelocity({ GetWorldScale().x * 1500, -1200 });
			EnableInput(false); rigid->SetGravityEnabled(false);
		});
	ani->defendAttack.Bind([this]()
		{
			GameModeHelper::PlayFXSound("sound_hardattack");
			AttackBox* attackBox = GameplayStatics::CreateObject<AttackBox>();
			attackBox->AttachTo(this);
			attackBox->SetLocalPosition(FVector2D(30, -30));
			attackBox->SetLocalScale(FVector2D(2.f, 1.5f));
			attackBox->Init(ECharacterDirection::LookForward, 6);
		});

	blinkTimes = 0;
}

void Player::BeginPlay()
{
	Super::BeginPlay();
	
	SetLocalPosition({ 0,900 });

	BlinkTimer.Bind(0.2f, [this]()
		{
			if (blinkTimes > 0)
			{
				if (bSitting)render->Blink(0.5f, WHITE, 90);
				else render->Blink(0.1f, BLACK);
				if (--blinkTimes == 0 && !bSitting)
				{
					hurtBox->SetCollisonMode(CollisionMode::Trigger);
					box->SetCollisionResponseToType(CollisionType::Bullet, true);
					GameModeHelper::GetInstance()->RefreshVolume();
					if (playerProperty->GetHealth() != 1)particle->SetIsLoop(false);
				}
			}
		}, true);

	LowHealthTimer.Bind(5.f, [this]()
		{
			ani->SetBool("lowHealth", GetHealth() <= 1);
		}, true);

	box->OnComponentHit.AddDynamic(this, &Player::OnEnter);
	box->OnComponentStay.AddDynamic(this, &Player::OnStay);

	particle->Deactivate();

	GameplayStatics::DontDestroyOnLoad(this);
	GameplayStatics::DontDestroyOnLoad(ui);
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);

	FVector2D cameraOffset;
	cameraOffset.x = 50.f;
	if (GetMovementState() == ECharacterMovementState::Standing)
	{
		if (direction == ECharacterDirection::LookDown && lookFlag > 1)
		{
			cameraOffset.y = 200; ani->SetInteger("lookFlag", 1);
		}
		else if (direction == ECharacterDirection::LookUp && lookFlag > 1)
		{
			cameraOffset.y = -200; ani->SetInteger("lookFlag", 2);
		}
	}
	else
	{
		lookFlag = 0;
	}
	if (lookFlag == 0)
	{
		ani->SetInteger("lookFlag", 0);
	}
	camera->SetLocalPosition(cameraOffset);



	if (box->IsCollisionsEmpty())
	{
		bGround = false; ani->SetBool("flying", true);
	}

	if (bWall)
	{
		if (bGround || box->IsCollisionsEmpty())
		{
			ani->SetTrigger("leaveWall");
		}
	}


	if (GetMovementState() != ECharacterMovementState::Running)
	{
		GameModeHelper::GetInstance()->GetAudioPlayer(1)->Stop("sound_swim");
		GameModeHelper::GetInstance()->GetAudioPlayer(1)->Stop("sound_waterwalk");
		GameModeHelper::GetInstance()->GetAudioPlayer(1)->Stop("sound_rush");
	}

	ani->SetFloat("walkingSpeed", FMath::Abs(rigid->GetVelocity().x));
	ani->SetFloat("landingSpeed", -1.f);
	ani->SetFloat("fallingSpeed", rigid->GetVelocity().y);
	if (GetHealth() > 1 || rigid->GetVelocity() != FVector2D::ZeroVector 
		|| !ani->IsPlaying("lowhealth") || direction != ECharacterDirection::LookForward)
	{
		ani->SetBool("lowHealth", false);
	}

	if (bEvading)
	{
		SetMaxWalkingSpeed(3000);
		AddInputX(-GetWorldScale().x * 4500 * deltaTime, false);
		if (GameplayStatics::GetTimeSeconds() - lastEvadeTime > 0.25f)
		{
			bEvading = false; 
		}
	}

	if (bDashing)
	{
		SetMaxWalkingSpeed(11000);
		AddInputX(GetWorldScale().x * (bGround ? 11000 : 10000) * deltaTime, false);
		if (GameplayStatics::GetTimeSeconds() - lastDashTime > 0.25f)
		{
			bDashing = false; 
			rigid->SetGravityEnabled(true);
			rigid->SetVelocity({ rigid->GetVelocity().x,0 });
		}
	}

	if (bFloating && GameplayStatics::GetTimeSeconds() - lastFloatTime > 1.2f)
	{
		ani->SetTrigger("floatingEnd");
	}
}

void Player::SetupInputComponent(InputComponent* inputComponent)
{
	inputComponent->SetMapping("WalkLeft", EKeyCode::VK_A);
	inputComponent->SetMapping("WalkRight", EKeyCode::VK_D);
	inputComponent->SetMapping("WalkLeftEnd", EKeyCode::VK_A);
	inputComponent->SetMapping("WalkRightEnd", EKeyCode::VK_D);
	inputComponent->SetMapping("Rush", EKeyCode::VK_Space);
	inputComponent->SetMapping("RushEnd", EKeyCode::VK_Space);
	inputComponent->SetMapping("LookUp", EKeyCode::VK_W);
	inputComponent->SetMapping("Sit", EKeyCode::VK_W);
	inputComponent->SetMapping("LookDownEnd", EKeyCode::VK_S);
	inputComponent->SetMapping("LookUpEnd", EKeyCode::VK_W);
	inputComponent->SetMapping("LookDown", EKeyCode::VK_S);
	inputComponent->SetMapping("JumpStart", EKeyCode::VK_K);
	inputComponent->SetMapping("Jumping", EKeyCode::VK_K);
	inputComponent->SetMapping("JumpEnd", EKeyCode::VK_K);
	inputComponent->SetMapping("Attack", EKeyCode::VK_J);
	inputComponent->SetMapping("Evade", EKeyCode::VK_L);
	inputComponent->SetMapping("Dash", EKeyCode::VK_F);
	inputComponent->SetMapping("Cure", EKeyCode::VK_E);
	inputComponent->SetMapping("Throw", EKeyCode::VK_Q);
	inputComponent->SetMapping("Leave", EKeyCode::VK_O);
	inputComponent->SetMapping("CloseSkill", EKeyCode::VK_I);
	inputComponent->SetMapping("RemoteSkill", EKeyCode::VK_O);
	inputComponent->SetMapping("RapidSkill", EKeyCode::VK_U);
	inputComponent->SetMapping("DefendStart", EKeyCode::VK_X);


	inputComponent->BindAction("WalkLeft", EInputType::Holding, [this]() {
		if (walkLock == 2 || bSitting || ani->IsPlaying("defend")) return;
		walkLock = 1;
		if (bWall)return;
		if (GetWorldScale().x > 0 && bGround)ani->PlayMontage("turn");
		if (!bDashing && !bEvading)
		{
			SetMaxWalkingSpeed(bRushing ? 700.f : 400.f);
		}		
		AddInputX(-3.f, !bWall);
		});
	inputComponent->BindAction("WalkLeftEnd", EInputType::Released, [this]() {
		if (walkLock == 1)ani->SetTrigger("leaveWall"); walkLock = 0; });
	inputComponent->BindAction("WalkRight", EInputType::Holding, [this]() {
		if (walkLock == 1 || bSitting || ani->IsPlaying("defend")) return;
		walkLock = 2;
		if (bWall)return;
		if (GetWorldScale().x < 0 && bGround)ani->PlayMontage("turn");
		if (!bDashing && !bEvading)
		{
			SetMaxWalkingSpeed(bRushing ? 700.f : 400.f);
		}
		AddInputX(3.f, !bWall);
		});
	inputComponent->BindAction("WalkRightEnd", EInputType::Released, [this]() {
		if (walkLock == 2)ani->SetTrigger("leaveWall"); walkLock = 0; });
	inputComponent->BindAction("Rush", EInputType::Holding, [this]() {
		if (bSitting) return;
		if (GetMovementState() == ECharacterMovementState::Running)bRushFlag = true;
		if (bRushFlag || bGround)bRushing = true; 
		}); 
	inputComponent->BindAction("RushEnd", EInputType::Released, [this]() {
		bRushing = false; bRushFlag = false;
		});
	inputComponent->BindAction("LookUp", EInputType::Holding, [this]() {
		if (ani->IsPlaying("defend")) return;
		if(direction == ECharacterDirection::LookForward)direction = ECharacterDirection::LookUp;
		if (GetMovementState() == ECharacterMovementState::Standing && lookFlag <= 1.5f)lookFlag += 0.0015f;
		});
	inputComponent->BindAction("Sit", EInputType::Pressed, [this]() {
		if (GetMovementState() == ECharacterMovementState::Standing && !bSitting)SitDown();
		});
	inputComponent->BindAction("LookUpEnd", EInputType::Released, [this]() {
		if (direction != ECharacterDirection::LookUp)return;
		direction = ECharacterDirection::LookForward; lookFlag = 0;
		});
	inputComponent->BindAction("LookDown", EInputType::Holding, [this]() {
		if (ani->IsPlaying("defend")) return;
		if (bSitting)ani->PlayMontage("standup");
		if (direction == ECharacterDirection::LookForward)direction = ECharacterDirection::LookDown;
		if (GetMovementState() == ECharacterMovementState::Standing && lookFlag <= 1.5f)lookFlag += 0.0015f;
		});
	inputComponent->BindAction("LookDownEnd", EInputType::Released, [this]() {
		if (direction != ECharacterDirection::LookDown)return;
		direction = ECharacterDirection::LookForward; lookFlag = 0;
		});
	inputComponent->BindAction("JumpStart", EInputType::Pressed, [this]() {
		if (bSitting) return;
		if (bGround)
		{
			bGround = false; SpawnWetLandEffect();
			rigid->AddImpulse({ 0,-600 });
			if (bRushing)ani->PlayMontage("rushjump");
			else ani->PlayMontage("jump");
			lastJumpTime = GameplayStatics::GetTimeSeconds();
			int32 jumpNum = FMath::RandInt(0, 10);
			if (jumpNum < 3)GameModeHelper::PlayFXSound("voice_jump_0");
			else if (jumpNum < 5)GameModeHelper::PlayFXSound("voice_jump_1");
			else if (jumpNum < 7)GameModeHelper::PlayFXSound("voice_jump_2");
			GameModeHelper::PlayFXSound("sound_jump");
			jumpFlag = 1;
		}
		else if (bWall)
		{
			AddPosition(FVector2D(GetWorldScale().x * 10.f, -10.f));
			rigid->AddImpulse(FVector2D(GetWorldScale().x * 500, -650)); 
			ani->PlayMontage("jump");
			lastJumpTime = GameplayStatics::GetTimeSeconds();
			int32 jumpNum = FMath::RandInt(0, 12);
			if (jumpNum < 2)GameModeHelper::PlayFXSound("voice_jump_0");
			else if (jumpNum < 4)GameModeHelper::PlayFXSound("voice_jump_1");
			else if (jumpNum < 6)GameModeHelper::PlayFXSound("voice_jump_2");
			GameModeHelper::PlayFXSound("sound_walljump");
			jumpFlag = 2;
		}
		});
	inputComponent->BindAction("Jumping", EInputType::Holding, [this]() {
		if (bSitting || bWall) return;
		if (rigid->GetVelocity().y < 0 && GameplayStatics::GetTimeSeconds() - lastJumpTime < 0.2f)
		{
			float delta = GameplayStatics::GetTimeSeconds() - lastJumpTime;
			bGround = false; ani->SetBool("flying", true); 
			rigid->AddImpulse(FVector2D(0, -(6.2f - FMath::Log(1 + delta < 0.06f ? 0 : FMath::Abs(rigid->GetVelocity().y)) / FMath::Log(10)) / jumpFlag));
		}
		});
	inputComponent->BindAction("JumpEnd", EInputType::Released, [this]() {
		if (bSitting || bWall) return;
		if (rigid->GetVelocity().y < 0 && GameplayStatics::GetTimeSeconds() - lastJumpTime < 0.3f)
		{
			rigid->SetVelocity(FVector2D(rigid->GetVelocity().x, -rigid->GetVelocity().y * 0.1f));
		}
		});
	inputComponent->BindAction("Attack", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (GameplayStatics::GetTimeSeconds() - lastAttackTime > 0.45f)
		{
			attackFlag = (attackFlag + 1) % 2;
			lastAttackTime = GameplayStatics::GetTimeSeconds();
			GameModeHelper::PlayFXSound("sound_attack");
			AttackBox* attackBox = GameplayStatics::CreateObject<AttackBox>();
			attackBox->AttachTo(this);
			switch (direction)
			{
				case ECharacterDirection::LookForward:
					if (bRushing && FMath::Abs(rigid->GetVelocity().x) > 500.f)
					{
						ani->PlayMontage("rushattack");
						attackBox->SetLocalPosition(FVector2D(110, 0));
					}
					else
					{
						if (attackFlag == 0)ani->PlayMontage("attack_0");
						else ani->PlayMontage("attack_1");
						attackBox->SetLocalPosition(FVector2D(60, 0));
					}
					break;
				case ECharacterDirection::LookUp:
					ani->PlayMontage("attackup"); attackBox->SetLocalPosition(FVector2D(20, 0));
					break;
				case ECharacterDirection::LookDown:
					ani->PlayMontage("attackdown");
					rigid->SetVelocity({}); ani->SetBool("validDownAttack", false);
					rigid->AddImpulse({ 500 * GetWorldScale().x, 775 });
					attackBox->GetComponentByClass<Collider>()->SetCollisonMode(CollisionMode::None);
					break;
			}
			attackBox->Init(direction);
		}
		});
	inputComponent->BindAction("Evade", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (bGround) 
		{ 
			ani->PlayMontage("evade"); bEvading = true; lastEvadeTime = GameplayStatics::GetTimeSeconds(); 
			GameModeHelper::PlayFXSound("sound_evade");
			int32 jumpNum = FMath::RandInt(0, 8);
			if (jumpNum < 3)GameModeHelper::PlayFXSound("sound_evade_0");
			else if (jumpNum < 5)GameModeHelper::PlayFXSound("sound_evade_1");
		}
		});
	inputComponent->BindAction("Dash", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (bGround)ani->PlayMontage("dash");
		else ani->PlayMontage("airdash");
		if (bWall)
		{
			AddPosition({ GetWorldScale().x * 25,-10 });
		}
		lastDashTime = GameplayStatics::GetTimeSeconds(); rigid->SetVelocity({ rigid->GetVelocity().x,0 });
		rigid->SetGravityEnabled(false); GameModeHelper::PlayFXSound("sound_dash");
		});
	inputComponent->BindAction("Cure", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;

		if (playerProperty->GetHealth() < 5)
		{
			if (playerProperty->GetSilk() >= 9 && playerProperty->GetHealth() < 4)
			{
				AddSilk(-9); AddHealth(3);
			}
			else if (playerProperty->GetSilk() >= 4)
			{
				AddSilk(-4); AddHealth(1);
			}
			else
			{
				return;
			}
			ani->PlayMontage("cure");
			camera->SetSpringArmLength(19); camera->ShakeCamera(5, 2);
			GameModeHelper::PlayFXSound("voice_cure");
			GameModeHelper::PlayFXSound("sound_cure");
			lastFloatTime = GameplayStatics::GetTimeSeconds() + 1.f;
			SetFloating(true);
			particle->SetIsLoop(false);
		}
		});
	inputComponent->BindAction("Throw", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (playerProperty->GetDart() <= 0 || GameplayStatics::GetTimeSeconds() - lastThrowTime < 0.5f)return;
		lastThrowTime = GameplayStatics::GetTimeSeconds();
		ani->PlayMontage("throw"); 
		GameModeHelper::PlayFXSound("sound_throw");
		if (FMath::RandInt(0, 10) > 5)GameModeHelper::PlayFXSound("voice_throw");
		});
	inputComponent->BindAction("Leave", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (playerProperty->GetSilk() < 3 || bGround)return; AddSilk(-3); 
		ani->PlayMontage("leavestart"); 
		GameModeHelper::PlayFXSound("sound_leave");
		EnableInput(false); rigid->SetGravityEnabled(false); rigid->SetVelocity({});
		GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D(GetLocalScale().x > 0 ? 200 : -200, -150),
			(GetLocalScale().x > 0 ? -150 : -30))->Init("effect_leave", -0.05f);
		});
	inputComponent->BindAction("CloseSkill", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (playerProperty->GetSilk() < 3) return; AddSilk(-3);
		ani->PlayMontage("_closeskill");
		GameplayStatics::CreateObject<CloseSkillBox>(GetWorldPosition());
		lastFloatTime = GameplayStatics::GetTimeSeconds(); SetFloating(true);
		GameModeHelper::PlayFXSound("sound_closeskill");
		if (FMath::RandInt(0, 10) > 5)GameModeHelper::PlayFXSound("voice_closeskill_0");
		else GameModeHelper::PlayFXSound("voice_closeskill_1");
		});
	inputComponent->BindAction("RemoteSkill", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (playerProperty->GetSilk() < 3 || !bGround)return; AddSilk(-3); EnableInput(false);
		ani->PlayMontage("remoteskill");
		if (FMath::RandInt(0, 10) > 5)GameModeHelper::PlayFXSound("voice_remoteskill_0");
		else GameModeHelper::PlayFXSound("voice_remoteskill_1");
		});
	inputComponent->BindAction("RapidSkill", EInputType::Pressed, [this]() {
		if (bSitting || bWall) return;
		if (playerProperty->GetSilk() < 3)return; AddSilk(-3); 
		lastFloatTime = GameplayStatics::GetTimeSeconds() + 1; SetFloating(true);
		ani->PlayMontage("rapidskill");
		});
	inputComponent->BindAction("DefendStart", EInputType::Pressed, [this]() {
		if (bSitting || bWall || !bGround || playerProperty->GetSilk() < 2) return;
		AddSilk(-2);
		ani->PlayMontage("defendstart");
		GameModeHelper::PlayFXSound("sound_defend");
		});
}


void Player::OnEnter(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (GetHealth() <= 0)
	{
		return;
	}

	if (normalImpulse.y < 0 && rigid->GetVelocity().y > 0)
	{
		bGround = true; ani->SetBool("flying", false);
		ani->SetFloat("landingSpeed", rigid->GetVelocity().y);
		SpawnWetLandEffect();
		if (GetWorldPosition().y > 1000)GameModeHelper::PlayFXSound("sound_waterland");
		if (rigid->GetVelocity().y > 1200)GameModeHelper::PlayFXSound("sound_hardland");
		else if(rigid->GetVelocity().y > 700)GameModeHelper::PlayFXSound("sound_softland");
		else GameModeHelper::PlayFXSound("sound_land");
	}
	else if (normalImpulse.x != 0 && GetWorldScale().x == -normalImpulse.x)
	{
		if (BoxCollider* platform = Cast<BoxCollider>(otherComp))
		{
			float delta_y = platform->GetWorldPosition().y - platform->GetSize().y * 0.5f - (box->GetWorldPosition().y + box->GetSize().y * 0.5f);
			if (delta_y > -box->GetSize().y)
			{
				rigid->SetGravityEnabled(false);
				box->SetPhysicsMaterial({});
				rigid->SetVelocity({ 0,3.75f * (delta_y > 0 ? 0 : delta_y) - 100.f });
				ani->PlayMontage("grab");
				GameModeHelper::PlayFXSound("sound_claw");
				EnableInput(false);
			}
		}
	}
}

void Player::OnStay(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (GetHealth() <= 0)
	{
		return;
	}

	if (normalImpulse.y < 0)
	{
		bGround = true; ani->SetBool("flying", false);
	}
	else if (normalImpulse.x != 0 && !bWall && !bGround && !bDashing &&
		((walkLock == 1 && normalImpulse.x == 1) || (walkLock == 2 && normalImpulse.x == -1)) && rigid->GetVelocity().y >= 0)
	{
		if (BoxCollider* platform = Cast<BoxCollider>(otherComp))
		{
			if (platform->GetWorldPosition().y - platform->GetSize().y * 0.5f < box->GetWorldPosition().y - box->GetSize().y * 0.5f
				&& platform->GetWorldPosition().y + platform->GetSize().y * 0.5f > box->GetWorldPosition().y + box->GetSize().y * 0.5f)
			{
				bWall = true;
				ani->SetNode("wall");
				rigid->SetVelocity({});
				rigid->SetGravityEnabled(false);
				SetLocalScale(FVector2D(normalImpulse.x, 1.f));
				GameModeHelper::PlayFXSound("sound_claw");
			}
		}
	}
}

FDamageCauseInfo Player::TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
{
	if (blinkTimes > 0)
	{
		return {};
	}
	FDamageCauseInfo damageInfo = damageResponse->TakeDamage(damageCauser, baseValue, damageType);
	AddHealth(-damageInfo.realValue);
	return damageInfo;
}

void Player::ExecuteDamageDealtEvent(FDamageCauseInfo extraInfo)
{

}

void Player::ExecuteDamageTakenEvent(FDamageCauseInfo extraInfo)
{
	if (!extraInfo.bIsValid)
	{
		return;
	}

	if (extraInfo.realValue == 0)
	{
		ani->PlayMontage("defendattack");
		GameModeHelper::PlayFXSound("sound_counter");
		Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D(0.f, 35.f));
		effect->Init("effect_counter");
		effect->SetLocalScale(GetWorldScale());
		return;
	}

	if (GetHealth() <= 0)
	{
		DieStart(); return;
	}

	blinkTimes = 10;
	bDashing = false;
	bEvading = false;
	SetFloating(false);

	ani->PlayMontage("hurt");
	particle->Activate();
	particle->SetIsLoop(true);
	rigid->SetGravityEnabled(true);
	rigid->SetVelocity({});
	Actor* causer = Cast<Actor>(extraInfo.damageCauser);
	CHECK_PTR(causer)
	rigid->AddImpulse({ (GetWorldPosition() - causer->GetWorldPosition()).GetSafeNormal().x * 200,-200 });
	hurtBox->SetCollisonMode(CollisionMode::None);
	box->SetCollisionResponseToType(CollisionType::Bullet, false);

	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	effect->Init("effect_hurt");
	effect = GameplayStatics::CreateObject<Effect>();
	effect->Init("effect_hurt_");
	effect->AttachTo(this);

	GameModeHelper::PlayFXSound("sound_hurt");
	int32 stunNum = FMath::RandInt(0, 10);
	if (stunNum < 3)GameModeHelper::PlayFXSound("sound_stun");
	else if (stunNum < 6)GameModeHelper::PlayFXSound("sound_stun_");

	GameModeHelper::GetInstance()->MakeEarRinging();
}

PropertyComponent* Player::GetProperty()
{
	if (!playerProperty)
	{
		playerProperty = ConstructComponent<PlayerPropertyComponent>();
	}
	return Cast<PropertyComponent>(playerProperty);
}

FVector2D Player::GetCameraPos()
{
	if (camera)return camera->GetVirtualPosition();
	return FVector2D();
}

int32 Player::GetHealth() const
{
	return playerProperty->GetHealth();
}

void Player::AddHealth(int32 delta)
{
	int32 initHealth = playerProperty->GetHealth();
	int32 realDelta = playerProperty->AddHealth(delta);

	if (realDelta > 0)
	{
		for (int i = 0; i < realDelta; i++)
		{
			ui->BloodLoad(initHealth + i);
		}
	}
	else
	{
		for (int i = 0; i > realDelta; i--)
		{
			ui->BloodMinus(initHealth + i - 1);
		}
	}
}

float Player::GetSilk() const
{
	return playerProperty->GetSilk();
}

void Player::AddSilk(int32 delta)
{
	int32 initSilk = playerProperty->GetSilk();
	int32 realDelta = playerProperty->AddSilk(delta);
	if (realDelta < 0 && initSilk == 9)ui->SoulMinus();
	else if (realDelta > 0 && playerProperty->GetSilk() == 9)ui->SoulLoad();

	if (realDelta > 0)
	{
		for (int i = 0; i < realDelta; i++)
		{
			ui->SilkLoad(initSilk + i);
		}
	}
	else
	{
		for (int i = 0; i > realDelta; i--)
		{
			ui->SilkMinus(initSilk + i - 1);
		}
	}
}

int32 Player::GetGeo() const
{
	return playerProperty->GetGeo();
}

void Player::AddGeo(int32 delta)
{
	playerProperty->AddGeo(delta);
}

int32 Player::GetDart() const
{
	return playerProperty->GetDart();
}

void Player::AddDart(int32 delta)
{
	playerProperty->AddDart(delta);
}

void Player::SetFloating(bool enable)
{
	bFloating = enable;
	rigid->SetGravityEnabled(!enable);
	rigid->SetMoveable(!enable);
	rigid->SetVelocity({});
	EnableInput(!enable);
	if(!enable)camera->SetSpringArmLength(20);
}

void Player::Bounce()
{
	ani->SetBool("validDownAttack",true);
	rigid->SetVelocity({});
	rigid->AddImpulse({ GetWorldScale().x * 450,-700 });
}

void Player::Grab()
{
	EnableInput(true);
	rigid->SetGravityEnabled(true);
	rigid->SetVelocity({ GetWorldScale().x * 75,0 });
	box->SetPhysicsMaterial(FPhysicsMaterial(0.1f, 0));
}

void Player::DieStart()
{
	EnableInput(false);
	particle->SetIsLoop(false);
	ani->SetNode("die");
	rigid->SetMoveable(false);
	rigid->SetGravityEnabled(false);
	GameplayStatics::CreateObject<DieParticle>()->AttachTo(this);
	DieTimer.Bind(3.f, this, &Player::DieEnd);
	GameModeHelper::PlayFXSound("voice_die");
	hurtBox->SetCollisonMode(CollisionMode::None);
}

void Player::DieEnd()
{
	ui->BlackInterval(true);
	rigid->SetMoveable(true);
	RecoverTimer.Bind(2.f, this, &Player::Recover);
	GameModeHelper::GetInstance()->MakeEarRinging();
}

void Player::Recover()
{
	EnableInput(true);
	GameplayStatics::OpenLevel("RuinHouse");
	SetLocalPosition({ 0,920 });
	bSitting = true; ani->SetNode("sitdown");
	rigid->SetVelocity({});
	ui->BlackInterval(false);
	AddHealth(5);
	AddSilk(9);
	AddDart(15);
	camera->SetRectFrame(FRect({ -100.f,250.f }, { 100.f,750.f }));
}

void Player::SitDown()
{
	if (Chair* chair = GameplayStatics::FindObjectOfClass<Chair>())
	{
		if (FVector2D::Distance(chair->GetWorldPosition(), GetWorldPosition()) >= 75)
		{
			return;
		}
		particle->SetIsLoop(false);
		SetLocalPosition(chair->GetWorldPosition() - FVector2D{ 0,30 });
		bSitting = true; ani->SetNode("sitdown"); blinkTimes = 1;
		GameModeHelper::PlayFXSound("sound_rest"); ui->WhiteBlink(3);
		GameplayStatics::CreateObject<SitParticle>(GetWorldPosition() + FVector2D(0, 45));
		Effect* effect = GameplayStatics::CreateObject<Effect>(FVector2D(-20, 0));
		effect->Init("effect_sit", -0.02f);
		effect->AttachTo(this);
		effect->SetLocalScale(GetWorldScale() * 0.5f);
		hurtBox->SetCollisonMode(CollisionMode::None);
		rigid->SetGravityEnabled(false);
		rigid->SetVelocity({});
		AddHealth(5);
	}
}


void Player::StandUp()
{
	bSitting = false;
	hurtBox->SetCollisonMode(CollisionMode::Trigger);
	rigid->SetGravityEnabled(true);
	GameModeHelper::PlayFXSound("sound_land");
}

void Player::LeaveUp()
{
	EnableInput(true);
	rigid->SetGravityEnabled(true);
	rigid->SetVelocity({});
}

void Player::LeaveWall()
{
	rigid->SetGravityEnabled(true);
	bWall = false;
}

void Player::Defend(bool enable)
{
	damageResponse->SetStrategy(enable ? new DamageStrategy() : new DefaultDamageStrategy());
}

void Player::Scare(bool enable, std::string anim)
{
	enable ? ani->SetNode(anim) : ani->SetNode("idle");
}

void Player::SpawnWetLandEffect() const
{
	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + FVector2D(0, 55));
	effect->Init("effect_wetland");
	effect->SetLocalScale(GetWorldScale());
}