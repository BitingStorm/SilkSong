#include "LevelTransformer.h"
#include "Components/Collider.h"
#include "Player.h"
#include "GameplayStatics.h"
#include "Components/Camera.h"


LevelTransformer::LevelTransformer()
{
	box = ConstructComponent<BoxCollider>();
	box->SetType(CollisionType::Block);
	box->AttachTo(root);
	box->SetSize({ 100,200 });
	box->OnComponentBeginOverlap.AddDynamic(this, &LevelTransformer::OnBeginOverlap);
}

void LevelTransformer::OnBeginOverlap(Collider* hitComp, Collider* otherComp, Actor* otherActor)
{
	if (Cast<Player>(otherActor))
	{
		GameplayStatics::OpenLevel(aimLevel);
		otherActor->SetLocalPosition(aimPos);
		if (Player* player = GameplayStatics::FindObjectOfClass<Player>())
		{
			player->GetComponentByClass<Camera>()->SetRectFrame(aimRect);
		}
	}
}

void LevelTransformer::Init(std::string level, FVector2D pos, FRect rec)
{
	aimLevel = level;
	aimPos = pos;
	aimRect = rec;
}