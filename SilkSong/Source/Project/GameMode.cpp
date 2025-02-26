#include "GameMode.h"


GameMode::GameMode()
{
	damageSystem = ConstructComponent<DamageSystem>();
	music = ConstructComponent<AudioPlayer>();
	sound = ConstructComponent<AudioPlayer>();
	music_ = ConstructComponent<AudioPlayer>();
}