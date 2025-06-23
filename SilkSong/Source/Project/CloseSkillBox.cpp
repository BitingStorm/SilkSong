#include "CloseSkillBox.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"
#include "Enemy.h"
#include "GameplayStatics.h"
#include "Chest.h"
#include "GameModeHelper.h"


CloseSkillBox::CloseSkillBox()
{
	render = ConstructComponent<SpriteRenderer>();
	render->SetLayer(3);
	SetRootComponent(render);
	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);
	effect.Load("effect_closeskill");
	effect.SetInterval(0.04f);
	ani->Insert("idle", effect);
	ani->SetNode("idle");


	circle = ConstructComponent<CircleCollider>();
	circle->AttachTo(root);
	circle->SetRadius(235);
	circle->SetType(CollisionType::HurtBox);


	DestroyTimerHandle.Bind(1.f, [this]() {Destroy(); }, false);
	AttackTimerHandle.Bind(0.15, [this]() {
		std::vector<Actor*> enemies = circle->GetCollisions(CollisionType::Enemy);
		for (auto& obj : enemies)
		{
			if (Enemy* enemy = Cast<Enemy>(obj))
			{
				if (enemy->IsDead())
				{
					continue;
				}
				GameModeHelper::ApplyDamage(this, enemy, 3, EDamageType::Player);
				GameModeHelper::PlayFXSound("sound_damage_0");
			}
		}
		std::vector<Actor*> chests = circle->GetCollisions(CollisionType::Chest);
		for (auto& obj : chests)
		{
			if (Chest* chest = Cast<Chest>(obj))
			{
				GameModeHelper::ApplyDamage(this, chest, 1, EDamageType::Player);
			}
		}
		}, true, 0.1f);
}