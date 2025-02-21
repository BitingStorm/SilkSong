#include "DefaultDamageStrategy.h"
#include "PropertyCarrier.h"
#include "PropertyComponent.h"
#include "Objects/Actor.h"


FDamageCauseInfo DefaultDamageStrategy::TakeDamage(IDamagable* damageCauser, float baseValue, EDamageType damageType)
{
    if (!Owner)
    {
        return{};
    }
    IPropertyCarrier* carrier = Cast<IPropertyCarrier>(Owner);
    CHECK_PTR(carrier)
    PropertyComponent* property = carrier->GetProperty();
    CHECK_PTR(property)
    
    if (property->GetHealth() <= 0)
    {
        return FDamageCauseInfo(false, damageCauser, 0);
    }
    else
    {
        return FDamageCauseInfo(true, damageCauser, baseValue);
    }     
}
