#include "CloseSkillBox.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Enemy.h"
#include "GameplayStatics.h"
#include "Chest.h"


CloseSkillBox::CloseSkillBox()
{
	render = ConstructComponent<SpriteRenderer>();
	render->SetLayer(3);
	SetRootComponent(render);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
	effect.Load("effect_closeskill");
	effect.SetInterval(0.06f);
	ani->Insert("idle", effect);
	ani->SetNode("idle");


	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetRadius(200);
	circle->SetType(CollisionType::HurtBox);


	DestroyTimerHandle.Bind(1.25f, [this]() {Destroy(); }, false);
	AttackTimerHandle.Bind(0.3f, [this]() {
		std::vector<Actor*> enemies = circle->GetCollisions(CollisionType::Enemy);
		for (auto& obj : enemies)
		{
			if (Enemy* enemy = Cast<Enemy>(obj))
			{
				if (enemy->IsDead())continue;
				enemy->TakeDamage((enemy->GetWorldPosition() - circle->GetWorldPosition()).Normalize(),false);
				GameplayStatics::PlaySound2D("sound_damage_0");
			}
		}
		std::vector<Actor*> chests = circle->GetCollisions(CollisionType::Chest);
		for (auto& obj : chests)
		{
			if (Chest* chest = Cast<Chest>(obj))
			{
				chest->TakeDamage();
			}
		}
		},true,0.1f);
}


