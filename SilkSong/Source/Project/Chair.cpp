#include "Chair.h"
#include "Components/Collider.h"
#include "Components/SpriteRenderer.h"


Chair::Chair()
{
	render = ConstructComponent<SpriteRenderer>();
	render->AttachTo(root);
	render->LoadSprite("chair");
	render->SetLayer(0);


	box = ConstructComponent<BoxCollider>();
	box->SetCollisonMode(CollisionMode::Trigger);
	box->SetType(CollisionType::Item);
	box->AttachTo(root);
}