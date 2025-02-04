#include "PlatForm.h"
#include"Components/SpriteRenderer.h"
#include"Components/Collider.h"


PlatForm::PlatForm()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	render->SetLayer(2);

	box = ConstructComponent<BoxCollider>();
	box->AttachTo(root);
	box->SetCollisonMode(CollisionMode::Collision);
	box->SetSize({500,50});
	box->SetType(CollisionType::Block);
}

void PlatForm::Init(std::string path, FVector2D size, FVector2D delta)
{
	if(path != "")render->LoadSprite(path); 
	box->SetSize(size);
	box->SetLocalPosition(delta);
}
