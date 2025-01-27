#include "Chest.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "AttackBox.h"
#include "CloseSkillBox.h"
#include "Needle.h"
#include "Geo.h"
#include "GameplayStatics.h"
#include "Tools/Math.h"
#include "RockParticle.h"
#include "Tools/Math.h"



Chest::Chest()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("chest");
	render->SetLayer(0);


	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetType(CollisionType::Chest);
	box->SetSize({100,140});

	box->OnComponentBeginOverlap.AddDynamic(this, &Chest::OnOverlap);

	num = 30;
}

void Chest::OnOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Cast<AttackBox>(otherActor) || Cast<Needle>(otherActor))
	{
		TakeDamage();
	}
}

void Chest::TakeDamage()
{
	int num = Math::RandInt(5, 8);
	this->num -= num;

	for (int i = 0; i < num; i++)
	{
		Geo* geo = GameplayStatics::CreateObject<Geo>(GetWorldPosition());
		if (Math::RandInt(0, 10) > 2)geo->Init("1geo");
		else if (Math::RandInt(0, 10) > 2)geo->Init("5geo");
		else geo->Init("8geo");
	}
	GameplayStatics::PlayCameraShake(5);
	GameplayStatics::PlaySound2D("sound_blockhit");

	if (this->num <= 0)
	{
		render->LoadSprite("chest_");
		render->SetLocalPosition({ 0,14 });
		box->SetCollisonMode(CollisionMode::None);
		GameplayStatics::PlayCameraShake(10);
		GameplayStatics::CreateObject<RockParticle>(GetWorldPosition());
	}
}
