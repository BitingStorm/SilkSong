#include "Struct.h"

FAttachmentTransformRules FAttachmentTransformRules::KeepRelativeTransform =
FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative);
FAttachmentTransformRules FAttachmentTransformRules::KeepWorldTransform =
FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld);
FAttachmentTransformRules FAttachmentTransformRules::KeepRelativeOnlyForLocation =
FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld);

FPhysicsMaterial FPhysicsMaterial::Combine(const FPhysicsMaterial& m1, const FPhysicsMaterial& m2, ECombinePattern pattern)
{
	if (pattern == ECombinePattern::Mean)
	{
		return (FPhysicsMaterial(m1) + m2) * 0.5f;
	}
	else
	{
		return pattern == ECombinePattern::Smaller ?
			FPhysicsMaterial(FMath::Min(m1.friction, m2.friction), FMath::Min(m1.bounciness, m2.bounciness))
			: FPhysicsMaterial(FMath::Max(m1.friction, m2.friction), FMath::Max(m1.bounciness, m2.bounciness));
	}
}
