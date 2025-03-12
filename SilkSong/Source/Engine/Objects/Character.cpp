#include "Character.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Components/RigidBody.h"
#include "Components/Camera.h"



Character::Character()
{
	movementState = ECharacterMovementState::Standing;
	bAddMoving = false;
	MoveFlag = 0;

	render = ConstructComponent<SpriteRenderer>();
	box = ConstructComponent<BoxCollider>();
	rigid = ConstructComponent<RigidBody>();

	SetRootComponent(render);
	Camera* camera = GetComponentByClass<Camera>();
	camera->AttachTo(root);
	box->AttachTo(root);
}

void Character::BeginPlay()
{
	Super::BeginPlay();

	box->OnComponentStay.AddDynamic(this, &Character::OnTouching);
}

void Character::Update(float deltaTime)
{
	Super::Update(deltaTime);

	if (box->IsCollisionsEmpty())
	{
		movementState = ECharacterMovementState::Flying;
	}

	if (MoveFlag > -1)
	{
		MoveFlag--;
	}

	if (MoveFlag == 0)
	{
		FVector2D vel = rigid->GetVelocity(); vel.x = 0;
		rigid->SetVelocity(vel);
	}
}

void Character::AddInputX(float inputValue, bool bControlScale)
{
	if (bControlScale)SetLocalScale({ (inputValue >= 0.f ? 1.f : -1.f),1.f });
	MoveFlag = 5;
	if (FMath::Abs(rigid->GetVelocity().x) >= maxWalkingSpeed)
	{
		rigid->SetVelocity({ rigid->GetVelocity().x / FMath::Abs(rigid->GetVelocity().x) * maxWalkingSpeed,rigid->GetVelocity().y }); return;
	}
	rigid->AddImpulse({ inputValue,0 });
}

void Character::OnTouching(Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const FHitResult& hitResult)
{
	if (normalImpulse.y < 0)
	{
		movementState = (rigid->GetVelocity().x == 0 ? ECharacterMovementState::Standing : ECharacterMovementState::Running);
	}
}