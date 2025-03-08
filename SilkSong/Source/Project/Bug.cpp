#include "Bug.h"
#include "Components/Collider.h"
#include "Components/RigidBody.h"
#include "Player.h"
#include "Geo.h"
#include "GameplayStatics.h"
#include "Components/AudioPlayer.h"
#include "PropertyComponent.h"
#include "GameModeHelper.h"



Bug::Bug()
{
	walk.Load("bug_walk");
	walk.SetInterval(0.1f);
	turn.Load("bug_turn");
	turn.SetInterval(0.1f);
	appear.Load("bug_appear");
	appear.SetInterval(0.1f);
    bury.Load("bug_bury");
	bury.SetInterval(0.1f);
	bury.SetLooping(false);
	die.Load("bug_die", {0,10});
	die.SetInterval(0.1f);
	die.SetLooping(false);

	turn_to_walk.Init(turn, walk);
	appear_to_walk.Init(appear, walk);
	walk_to_bury.Init(walk, bury);
	walk_to_bury.AddCondition(AnimTransition::Bool{ "burying",true });
	bury_to_appear.Init(bury, appear);
	bury_to_appear.AddCondition(AnimTransition::Bool{ "burying",false });
	walk_to_die.Init(walk, die);
	walk_to_die.AddCondition(AnimTransition::Bool{ "dead",true });


	ani->Insert("walk", walk);
	ani->Insert("turn", turn);
	ani->Insert("appear", appear);
	ani->Insert("bury", bury);
	ani->Insert("die", die);
	ani->SetNode("walk");

	ani->AddParamater("burying", ParamType::Bool);
	ani->AddParamater("dead", ParamType::Bool);

	StateUpdateTimerHandle.Bind(3.f, [this]() {
		if (player && rigid->GetVelocity().y == 0 && !IsDead())
		{
			if (FVector2D::Distance(player->GetWorldPosition(), GetWorldPosition()) > 350 &&
				FMath::Abs(circle->GetWorldPosition().x - currentPlatForm->GetWorldPosition().x) < currentPlatForm->GetSize().x * 0.5f - 25)
			{
				bIsBuried = true;
				circle->SetCollisonMode(CollisionMode::None);
				rigid->SetMoveable(false);
			}
			else if(bIsBuried)
			{
				circle->SetCollisonMode(CollisionMode::Collision);
				rigid->SetMoveable(true);
				if (FMath::RandInt(0, 10) > 5)GameModeHelper::PlayFXSound("sound_bug_appear");
				else GameModeHelper::PlayFXSound("sound_bug_appear_");
				bIsBuried = false;
			}
		}
		},true);
}


void Bug::BeginPlay()
{
	Super::BeginPlay();

	circle->OnComponentHit.AddDynamic(this, &Bug::OnHit);

	property->SetMaxHealth(10);
	property->AddHealth(10);
}

void Bug::Update(float deltaTime)
{
	Super::Update(deltaTime);

	ani->SetBool("burying", bIsBuried);
	ani->SetBool("dead", IsDead());

	if (bIsBuried || IsDead())return;

	rigid->AddImpulse({ GetWorldScale().x * 500 * deltaTime,0 });
	if (FMath::Abs(rigid->GetVelocity().x) > 100)
	{
		FVector2D newVel = rigid->GetVelocity();
		newVel.x *= 0.95f;
		rigid->SetVelocity(newVel);
	}

	if (circle->IsCollisionsEmpty())currentPlatForm = nullptr;

	if (currentPlatForm) 
	{
		float delta = circle->GetWorldPosition().x - currentPlatForm->GetWorldPosition().x;
		if (FMath::Abs(delta) >= currentPlatForm->GetSize().x * 0.5f - 5 && FMath::Abs(delta) < currentPlatForm->GetSize().x * 0.5f
			&& delta * GetLocalScale().x > 0)
		{
			rigid->SetVelocity(-rigid->GetVelocity());
			AddPosition(rigid->GetVelocity() * deltaTime);
			SetLocalScale({ -GetLocalScale().x,GetLocalScale().y });
			ani->PlayMontage("turn");
		}
	}
}

void Bug::OnHit(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (hitComp->GetType() == CollisionType::Player || IsDead())return;

	if (normalImpulse.y < 0)
	{
		if (BoxCollider* collider = Cast<BoxCollider>(otherComp))
		{
			currentPlatForm = collider;
		}
	}

	if (normalImpulse.x != 0)
	{
		SetLocalScale({ -GetLocalScale().x,GetLocalScale().y });
		ani->PlayMontage("turn");
	}
}

void Bug::SpawnGeos()
{
	for (int i = 0; i < 3; i++)
	{
		Geo* geo = GameplayStatics::CreateObject<Geo>(GetWorldPosition());
		geo->Init("1geo", 1);
	}
}

void Bug::Die()
{
	Super::Die();
	GameModeHelper::PlayFXSound("sound_bug_die");
}
