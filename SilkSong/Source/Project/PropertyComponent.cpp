#include "PropertyComponent.h"


PropertyComponent::PropertyComponent()
{
	health = 0;
	maxHealth = 0;
}

int32 PropertyComponent::AddHealth(int32 delta)
{
	int32 initial = health;
	health += delta;
	health = FMath::Clamp(health, 0, maxHealth);
	return health - initial;
}
