#include "Brumm.h"
#include "Components/SpriteRenderer.h"
#include "Components/AudioPlayer.h"
#include "GameModeHelper.h"


Brumm::Brumm()
{
	render = ConstructComponent<SpriteRenderer>();
	SetRootComponent(render);
	render->SetLayer(-1);

	ani = ConstructComponent<Animator>();
	ani->SetupAttachment(render);

	idle.Load("brumm");
	idle.SetInterval(0.08f);

	ani->Insert("idle", idle);
	ani->SetNode("idle");

	audio = ConstructComponent<AudioPlayer>();
	audio->AttachTo(root);
	audio->SetSpacial(true); 
	audio->SetRadius(200, 1000); 
	audio->SetVolume(GameModeHelper::GetInstance()->GetVolume(1));
	audio->Play("accordion", true);
}