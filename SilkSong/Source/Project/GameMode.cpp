#include "GameMode.h"
#include "DamageSystem.h"


GameMode::GameMode()
{
	damageSystem = ConstructComponent<DamageSystem>();
}
