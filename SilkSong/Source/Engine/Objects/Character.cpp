#include "Character.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Components/RigidBody.h"
#include "Components/Camera.h"



Character::Character()
{
	movementState = CharacterMovementState::Standing;
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
		movementState = CharacterMovementState::Flying;
	}

	if (MoveFlag > -1)
	{
		MoveFlag--;
	}

	if (MoveFlag == 0)
	{
		Vector2D vel = rigid->GetVelocity(); vel.x = 0;
		rigid->SetVelocity(vel);
	}
}

void Character::AddInputX(float inputValue, bool bControlScale)
{
	if (bControlScale)SetLocalScale({ (inputValue >= 0.f ? 1.f : -1.f),1.f });
	MoveFlag = 5;
	if (std::abs(rigid->GetVelocity().x) >= maxWalkingSpeed)
	{
		rigid->SetVelocity({ rigid->GetVelocity().x / std::abs(rigid->GetVelocity().x) * maxWalkingSpeed,rigid->GetVelocity().y }); return;
	}
	rigid->AddImpulse({ inputValue,0 });
}

void Character::OnTouching(Collider* hitComp, Collider* otherComp, Actor* otherActor, Vector2D normalImpulse, const HitResult& hitResult)
{
	if (normalImpulse.y < 0)
	{
		movementState = (rigid->GetVelocity().x == 0 ? CharacterMovementState::Standing : CharacterMovementState::Running);
	}
}


