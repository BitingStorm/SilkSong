#include "Player.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Components/RigidBody.h"
#include "Components/Camera.h"
#include "Components/AudioPlayer.h"
#include "GameplayStatics.h"
#include "Tools/Math.h"

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
	box->SetSize({40,130});
	box->SetLocalPosition({-5,10});
	box->SetCollisonMode(CollisionMode::Collision);
	box->SetType(CollisionType::Player);
	box->SetPhysicsMaterial(PhysicsMaterial(0.1f,0));

	hurtBox = ConstructComponent<BoxCollider>();
	hurtBox->AttachTo(root);
	hurtBox->SetSize({ 30,100 });
	hurtBox->SetLocalPosition({ -5,10 });
	hurtBox->SetType(CollisionType::HurtBox);


	rigid = GetComponentByClass<RigidBody>();
	rigid->SetLinearDrag(0.07f);
	rigid->SetGraivty(1960);

	camera = GetComponentByClass<Camera>();
	camera->SetDistanceThreshold(100);
	camera->SetSmoothness(50);
	camera->SetRectFrame({-375,725,1125,-1000});

	audio = ConstructComponent<AudioPlayer>();
	audio->AttachTo(root);


	bGround = false;
	bDashing = false;
	bEvading = false;
	bFloating = false;
	bRushing = false;
	bRushFlag = false;
	bSitting = false;

	lastJumpTime = 0.f;
	lastAttackTime = 0.f;
	lastEvadeTime = 0.f;
	lastDashTime = 0.f;
	lastFloatTime = 0.f;
	lastThrowTime = 0.f;
	
	
	attackFlag = 0;
	lookFlag = 0;
	direction = ECharacterDirection::LookForward;

	ani->dashEffect.Bind(this, &Player::SpawnDashEffect);
	ani->cureEffect.Bind([this]()
		{
			AddHealth(3); GameplayStatics::CreateObject<HealParticle>()->AttachTo(this);
			ui->WhiteBlink();
		});
	ani->wetWalkEffect.Bind(this, &Player::SpawnWetWalkEffect);
	ani->dartSpawn.Bind([this]()
		{
			Dart* dart = GameplayStatics::CreateObject<Dart>(GetWorldPosition());
			if (dart)dart->Init(GetWorldScale().x < 0);
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
			if (!effect)return;
			effect->Init("effect_throw", -0.01f); effect->SetLocalScale(GetLocalScale());
			effect->AddPosition({GetLocalScale().x * 50,0});
			AddDart(-1);
		});
	ani->needleSpawn.Bind([this]()
		{
			Needle* needle = GameplayStatics::CreateObject<Needle>({ GetLocalScale().x * -500,0 });
			Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
			if (!effect || !needle)return;
			needle->AttachTo(effect);
			effect->Init("effect_remoteskill"); effect->SetLocalScale(GetLocalScale());
			effect->AddPosition({ GetLocalScale().x * 500,25 });
			audio->Play("sound_remoteskill");
		});
	ani->grabFinished.Bind(this, &Player::Grab);
	ani->downAttackSpawn.Bind([this]()
		{
			AttackBox* attackBox = GameplayStatics::CreateObject<AttackBox>();
			attackBox->AttachTo(this);
			attackBox->SetLocalPosition(Vector2D(70 * GetWorldScale().x, 35));
		});
	ani->leaveStart.Bind([this]()
		{
			rigid->SetVelocity({ GetWorldScale().x * 840, -630 });
		});

	blinkTimes = 0;
	health = 5;
	soul = 9;
	geo = 0;
	dartNum = 15;
}

void Player::BeginPlay()
{
	Super::BeginPlay();

	BlinkTimer.Bind(0.2f, [this]()
		{
			if (blinkTimes > 0)
			{
				if (bSitting)render->Blink(0.5f, WHITE, 90);
				else render->Blink(0.1f, BLACK);
				if (--blinkTimes == 0 && !bSitting)hurtBox->SetCollisonMode(CollisionMode::Trigger);
			}
		}, true);

	box->OnComponentHit.AddDynamic(this, &Player::OnEnter);
	box->OnComponentStay.AddDynamic(this, &Player::OnStay);

	ui = GameplayStatics::CreateUI<GameUI>();
	ui->AddToViewport();
}

void Player::Update(float deltaTime)
{
	Super::Update(deltaTime);
	
	Vector2D cameraOffset;
	cameraOffset.x = (GetWorldScale().x == 1.f ? 50.f : -50.f);
	if (GetMovementState() == CharacterMovementState::Standing)
	{
		if (direction == ECharacterDirection::LookDown && lookFlag > 1)cameraOffset.y = 200;
		else if (direction == ECharacterDirection::LookUp && lookFlag > 1)cameraOffset.y = -200;
	}
	else
	{
		lookFlag = 0;
	}	
	camera->SetLocalPosition(cameraOffset);


	if (box->IsCollisionsEmpty())
	{
		bGround = false; ani->SetBool("flying", true);
	}
	
	if (GetMovementState() != CharacterMovementState::Running)
	{
		if (GetWorldPosition().y > 1000)audio->Stop("sound_swim");
		else audio->Stop("sound_waterwalk");
	}

	ani->SetFloat("walkingSpeed", std::abs(rigid->GetVelocity().x));
	ani->SetFloat("landingSpeed", -1.f);
	ani->SetFloat("fallingSpeed", rigid->GetVelocity().y);

	if (bEvading)
	{
		SetMaxWalkingSpeed(3000);
		AddInputX(-GetWorldScale().x * 3000 * deltaTime, false);
		if (GameplayStatics::GetTimeSeconds() - lastEvadeTime > 0.35f)
		{
			bEvading = false; 
		}
	}

	if (bDashing)
	{
		SetMaxWalkingSpeed(10000);
		AddInputX(GetWorldScale().x * (bGround ? 12500 : 10000) * deltaTime, false);
		if (GameplayStatics::GetTimeSeconds() - lastDashTime > 0.3f)
		{
			bDashing = false; 
			rigid->SetGravityEnabled(true);
			rigid->SetVelocity({ rigid->GetVelocity().x,0 });
		}
	}

	if (bFloating)
	{
		if (GameplayStatics::GetTimeSeconds() - lastFloatTime > 1.5f)
		{
			ani->SetTrigger("floatingEnd");
		}
	}
}

void Player::SetupInputComponent(InputComponent* inputComponent)
{

	inputComponent->SetMapping("WalkLeft", KeyCode::VK_A);
	inputComponent->SetMapping("WalkRight", KeyCode::VK_D);
	inputComponent->SetMapping("WalkLeftEnd", KeyCode::VK_A);
	inputComponent->SetMapping("WalkRightEnd", KeyCode::VK_D);
	inputComponent->SetMapping("Rush", KeyCode::VK_Space);
	inputComponent->SetMapping("RushEnd", KeyCode::VK_Space);
	inputComponent->SetMapping("LookUp", KeyCode::VK_W);
	inputComponent->SetMapping("Sit", KeyCode::VK_W);
	inputComponent->SetMapping("LookDownEnd", KeyCode::VK_S);
	inputComponent->SetMapping("LookUpEnd", KeyCode::VK_W);
	inputComponent->SetMapping("LookDown", KeyCode::VK_S);
	inputComponent->SetMapping("JumpStart", KeyCode::VK_K);
	inputComponent->SetMapping("Jumping", KeyCode::VK_K);
	inputComponent->SetMapping("Attack", KeyCode::VK_J);
	inputComponent->SetMapping("Evade", KeyCode::VK_L);
	inputComponent->SetMapping("Dash", KeyCode::VK_F);
	inputComponent->SetMapping("Cure", KeyCode::VK_E);
	inputComponent->SetMapping("Throw", KeyCode::VK_Q);
	inputComponent->SetMapping("Leave", KeyCode::VK_O);
	inputComponent->SetMapping("CloseSkill", KeyCode::VK_I);
	inputComponent->SetMapping("RemoteSkill", KeyCode::VK_O);


	inputComponent->BindAction("WalkLeft", InputType::Holding, [this]() {
		if (walkLock == 2 || bSitting) return; 
		walkLock = 1;
		if (GetWorldScale().x > 0 && bGround)ani->PlayMontage("turn");
		if (!bDashing && !bEvading)
		{
			SetMaxWalkingSpeed(bRushing ? 700.f : 400.f);
		}
		AddInputX(-2.5);
		});
	inputComponent->BindAction("WalkLeftEnd", InputType::Released, [this]() {walkLock = 0; });
	inputComponent->BindAction("WalkRight", InputType::Holding, [this]() {
		if (walkLock == 1 || bSitting) return;
		walkLock = 2;
		if (GetWorldScale().x < 0 && bGround)ani->PlayMontage("turn");
		if (!bDashing && !bEvading)
		{
			SetMaxWalkingSpeed(bRushing ? 700.f : 400.f);
		}
		AddInputX(2.5);
		});
	inputComponent->BindAction("WalkRightEnd", InputType::Released, [this]() {walkLock = 0; });
	inputComponent->BindAction("Rush", InputType::Holding, [this]() {
		if (bSitting) return;
		if (GetMovementState() == CharacterMovementState::Running)bRushFlag = true;
		if (bRushFlag || bGround)bRushing = true; 
		}); 
	inputComponent->BindAction("RushEnd", InputType::Released, [this]() {
		bRushing = false; bRushFlag = false;
		});
	inputComponent->BindAction("LookUp", InputType::Holding, [this]() {
		if(direction == ECharacterDirection::LookForward)direction = ECharacterDirection::LookUp;
		if (GetMovementState() == CharacterMovementState::Standing && lookFlag <= 1.5f)lookFlag += 0.0015f;
		});
	inputComponent->BindAction("Sit", InputType::Pressed, [this]() {
		if (GetMovementState() == CharacterMovementState::Standing && !bSitting)SitDown();
		});
	inputComponent->BindAction("LookUpEnd", InputType::Released, [this]() {
		if (direction != ECharacterDirection::LookUp)return;
		direction = ECharacterDirection::LookForward; lookFlag = 0;
		});
	inputComponent->BindAction("LookDown", InputType::Holding, [this]() {
		if (bSitting)ani->PlayMontage("standup");
		if (direction == ECharacterDirection::LookForward)direction = ECharacterDirection::LookDown;
		if (GetMovementState() == CharacterMovementState::Standing && lookFlag <= 1.5f)lookFlag += 0.0015f;
		});
	inputComponent->BindAction("LookDownEnd", InputType::Released, [this]() {
		if (direction != ECharacterDirection::LookDown)return;
		direction = ECharacterDirection::LookForward; lookFlag = 0;
		});
	inputComponent->BindAction("JumpStart", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (bGround)
		{
			bGround = false; SpawnWetLandEffect(); 
			rigid->AddImpulse({ 0,-400 });
			if (bRushing)ani->PlayMontage("rushjump");
			else ani->PlayMontage("jump"); 
			lastJumpTime = GameplayStatics::GetTimeSeconds();
			int32 jumpNum = Math::RandInt(0, 10);
			if(jumpNum < 3)audio->Play("voice_jump_0");
			else if(jumpNum < 5)audio->Play("voice_jump_1");
			else if (jumpNum < 7)audio->Play("voice_jump_2");
			audio->Play("sound_jump");
		}
		});
	inputComponent->BindAction("Jumping", InputType::Holding, [this]() {
		if (bSitting) return;
		if (rigid->GetVelocity().y <= 0 && GameplayStatics::GetTimeSeconds() - lastJumpTime < 0.25f)
		{
			bGround = false; ani->SetBool("flying", true);
			rigid->AddImpulse({ 0,-4.f }); 
		}
		});
	inputComponent->BindAction("Attack", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (GameplayStatics::GetTimeSeconds() - lastAttackTime > 0.45f)
		{
			attackFlag = (attackFlag + 1) % 2;
			lastAttackTime = GameplayStatics::GetTimeSeconds();
			audio->Play("sound_attack");
			AttackBox* attackBox = GameplayStatics::CreateObject<AttackBox>();
			attackBox->AttachTo(this);
			switch (direction)
			{
				case ECharacterDirection::LookForward:
					if (attackFlag == 0)ani->PlayMontage("attack_0");
					else ani->PlayMontage("attack_1");
					attackBox->SetLocalPosition(Vector2D(70 * GetWorldScale().x, 0));
					break;
				case ECharacterDirection::LookUp:
					ani->PlayMontage("attackup"); attackBox->SetLocalPosition(Vector2D(20 * GetWorldScale().x, 0));
					break;
				case ECharacterDirection::LookDown:
					ani->PlayMontage("attackdown");
					rigid->SetVelocity({}); ani->SetBool("validDownAttack", false);
					rigid->AddImpulse({ 475 * GetWorldScale().x, 725 });
					break;
			}
			attackBox->Init(direction);
		}
		});
	inputComponent->BindAction("Evade", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (bGround) 
		{ 
			ani->PlayMontage("evade"); bEvading = true; lastEvadeTime = GameplayStatics::GetTimeSeconds(); 
			audio->Play("sound_evade");
			int32 jumpNum = Math::RandInt(0, 8);
			if (jumpNum < 3)audio->Play("sound_evade_0");
			else if (jumpNum < 5)audio->Play("sound_evade_1");
		}
		});
	inputComponent->BindAction("Dash", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (bGround)ani->PlayMontage("dash");
		else ani->PlayMontage("airdash");
		lastDashTime = GameplayStatics::GetTimeSeconds(); rigid->SetVelocity({ rigid->GetVelocity().x,0 });
		rigid->SetGravityEnabled(false); audio->Play("sound_dash");
		});
	inputComponent->BindAction("Cure", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (soul >= 9 && health < 5) 
		{
			AddSoul(-9);
			ani->PlayMontage("cure");   
			camera->SetSpringArmLength(19); camera->ShakeCamera(5, 2);
			audio->Play("voice_cure");
			audio->Play("sound_cure");
			lastFloatTime = GameplayStatics::GetTimeSeconds() + 1.f; 
			SetFloating(true);
		}
		});
	inputComponent->BindAction("Throw", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (dartNum <= 0 || GameplayStatics::GetTimeSeconds() - lastThrowTime < 0.5f)return;
		lastThrowTime = GameplayStatics::GetTimeSeconds();
		ani->PlayMontage("throw"); audio->Play("sound_throw");
		if (Math::RandInt(0, 10) > 5)audio->Play("voice_throw");
		});
	inputComponent->BindAction("Leave", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (soul < 3 || bGround)return; AddSoul(-3); EnableInput(false);
		lastThrowTime = GameplayStatics::GetTimeSeconds();
		ani->PlayMontage("leave"); audio->Play("sound_leave");
		rigid->SetGravityEnabled(false);rigid->SetVelocity({});
		GameplayStatics::CreateObject<Effect>(GetWorldPosition() + Vector2D(GetLocalScale().x > 0 ? 200 : -200, -150),
			(GetLocalScale().x > 0 ? -150 : -30))->Init("effect_leave", -0.02f);
		});
	inputComponent->BindAction("CloseSkill", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (soul < 3) return; AddSoul(-3);
		ani->PlayMontage("_closeskill");
		GameplayStatics::CreateObject<CloseSkillBox>(GetWorldPosition());
		lastFloatTime = GameplayStatics::GetTimeSeconds(); SetFloating(true);
		audio->Play("sound_closeskill");
		if (Math::RandInt(0, 10) > 5)audio->Play("voice_closeskill_0");
		else audio->Play("voice_closeskill_1");
		});
	inputComponent->BindAction("RemoteSkill", InputType::Pressed, [this]() {
		if (bSitting) return;
		if (soul < 3 || !bGround)return; AddSoul(-3); EnableInput(false);
		ani->PlayMontage("remoteskill");
		if (Math::RandInt(0, 10) > 5)audio->Play("voice_remoteskill_0");
		else audio->Play("voice_remoteskill_1");
		});
}


void Player::OnEnter(Collider* hitComp, Collider* otherComp, Actor* otherActor, Vector2D normalImpulse, const HitResult& hitResult)
{
	if (normalImpulse.y < 0)
	{
		bGround = true; ani->SetBool("flying", false);
		ani->SetFloat("landingSpeed", rigid->GetVelocity().y);
		SpawnWetLandEffect();
		if (GetWorldPosition().y > 1000)audio->Play("sound_waterland");
		if (rigid->GetVelocity().y > 1200)GameplayStatics::PlaySound2D("sound_hardland");
		else if(rigid->GetVelocity().y > 500)GameplayStatics::PlaySound2D("sound_softland");
		else GameplayStatics::PlaySound2D("sound_land");
	}
	else if (normalImpulse.x != 0 && GetWorldScale().x == -normalImpulse.x)
	{
		if (bDashing)
		{
			return;
		}
		if (BoxCollider* platform = Cast<BoxCollider>(otherComp))
		{
			float delta_y = platform->GetWorldPosition().y - platform->GetSize().y * 0.5f - (GetWorldPosition().y + box->GetSize().y * 0.5f);
			if (delta_y > -box->GetSize().y)
			{
				rigid->SetGravityEnabled(false);
				box->SetPhysicsMaterial({});
				rigid->SetVelocity({ 0,3.75f * (delta_y > 0 ? 0 : delta_y) - 100.f });
				ani->PlayMontage("grab");
				GameplayStatics::PlaySound2D("sound_claw");
				EnableInput(false);
			}
		}
	}
}

void Player::OnStay(Collider* hitComp, Collider* otherComp, Actor* otherActor, Vector2D normalImpulse, const HitResult& hitResult)
{
	if (normalImpulse.y < 0)
	{
		bGround = true; ani->SetBool("flying", false);
	}
}

void Player::TakeDamage(Vector2D normal)
{
	if (blinkTimes > 0 || health == 0)
	{
		return;
	}
	AddHealth(-1);
	if (health == 0)
	{
		Die(); return;
	}
	blinkTimes = 10;

	ani->PlayMontage("hurt");
	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition());
	effect->Init("effect_hurt");
	bDashing = false;
	bEvading = false;
	bFloating = false;
	rigid->SetGravityEnabled(true);
	rigid->SetVelocity({});
	rigid->AddImpulse({ normal.x * 200,-200 });
	hurtBox->SetCollisonMode(CollisionMode::None);
	effect = GameplayStatics::CreateObject<Effect>();
	effect->Init("effect_hurt_");
	effect->AttachTo(this);

	int32 stunNum = Math::RandInt(0, 10);
	audio->Play("sound_hurt");
	if (stunNum < 3)audio->Play("sound_stun");
	else if (stunNum < 6)audio->Play("sound_stun_");
}

Vector2D Player::GetCameraPos()
{
	if (camera)return camera->GetVirtualPosition();
	return Vector2D();
}

void Player::AddHealth(int32 delta)
{
	health += delta;
	health = Math::Clamp(health, 0, 5);
}

void Player::AddSoul(float delta)
{
	soul += delta;
	soul = Math::Clamp(soul, 0.f, 9.f);
}

void Player::AddDart(int32 delta)
{
	dartNum += delta;
	dartNum = Math::Clamp(dartNum, 0, 999);
}

void Player::SetFloating(bool enable)
{
	bFloating = enable;
	rigid->SetGravityEnabled(!enable);
	rigid->SetMoveable(!enable);
	rigid->SetVelocity({});
	EnableInput(!enable);
}

void Player::Bounce()
{
	ani->SetBool("validDownAttack",true);
	rigid->SetVelocity({});
	rigid->AddImpulse({ GetWorldScale().x * 420,-660 });
}

void Player::Grab()
{
	EnableInput(true);
	rigid->SetGravityEnabled(true);
	rigid->SetVelocity({ GetWorldScale().x * 75,0 });
	box->SetPhysicsMaterial(PhysicsMaterial(0.1f, 0));
}

void Player::Die()
{
	EnableInput(false);
	ani->SetNode("die");
	rigid->SetVelocity({ 0,-20 });
	rigid->SetGravityEnabled(false);
	GameplayStatics::CreateObject<DieParticle>()->AttachTo(this);
	RecoverTimer.Bind(3,this,&Player::Recover);
	audio->Play("sound_die");
	audio->Play("voice_die");
	hurtBox->SetCollisonMode(CollisionMode::None);
}

void Player::Recover()
{
	EnableInput(true);
	if (Chair* chair = GameplayStatics::FindObjectOfClass<Chair>())
	{
		SetLocalPosition(chair->GetWorldPosition() - Vector2D{ 0,30 });
		bSitting = true; ani->SetNode("sitdown"); health = 5;
		rigid->SetVelocity({});
	}
}

void Player::SitDown()
{
	if (Chair* chair = GameplayStatics::FindObjectOfClass<Chair>())
	{
		if (Vector2D::Distance(chair->GetWorldPosition(), GetWorldPosition()) >= 75)
		{
			return;
		}
		SetLocalPosition(chair->GetWorldPosition() - Vector2D{ 0,30 });
		bSitting = true; ani->SetNode("sitdown"); health = 5; blinkTimes = 1;
		audio->Play("sound_heal"); ui->WhiteBlink();
		GameplayStatics::CreateObject<SitParticle>(GetWorldPosition() + Vector2D(0, 45));
		Effect* effect = GameplayStatics::CreateObject<Effect>(Vector2D(-20, 0));
		effect->Init("effect_sit", -0.02f);
		effect->AttachTo(this);
		effect->SetLocalScale(GetWorldScale() * 0.5f);
		hurtBox->SetCollisonMode(CollisionMode::None);
		rigid->SetGravityEnabled(false);
		rigid->SetVelocity({});
	}
}


void Player::StandUp()
{
	bSitting = false;
	hurtBox->SetCollisonMode(CollisionMode::Trigger);
	rigid->SetGravityEnabled(true);
}

void Player::LeaveUp()
{
	EnableInput(true);
	rigid->SetGravityEnabled(true);
	rigid->SetVelocity({});
}


void Player::SpawnDashEffect()
{
	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() - Vector2D(GetWorldScale().x * 150, 0));
	if (!effect)return;
	if (bGround)
	{
		effect->Init("effect_dash"); effect->SetLocalScale(GetWorldScale());
	}
	else
	{
		effect->Init("effect_dash_", -0.03f); effect->SetLocalScale({ -GetWorldScale().x,1.f }); effect->AddPosition({ -GetWorldScale().x * 125,-50});
	}
	bDashing = true;
}

void Player::SpawnWetLandEffect()
{
	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + Vector2D(0, 55));
	if (!effect)return;
	effect->Init("effect_wetland");
	effect->SetLocalScale(GetWorldScale());
}

void Player::SpawnWetWalkEffect()
{
	Effect* effect = GameplayStatics::CreateObject<Effect>(GetWorldPosition() + Vector2D(0, 60));
	if (!effect)return;
	effect->Init("effect_wetwalk");
	effect->SetLocalScale(GetWorldScale() * Math::RandReal(0.8f, 1.1f));
}
