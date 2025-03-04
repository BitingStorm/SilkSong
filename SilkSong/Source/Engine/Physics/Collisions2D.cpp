#include "Collisions2D.h"
#include "Components/Collider.h"


bool Physics2D::Collisions2D::CollisionTestCircleToCircle(Collider* c1, Collider* c2, FHitResult& hitResult)
{
    return FVector2D::DistSquared(c1->GetWorldPosition(), c2->GetWorldPosition())
        <= FMath::Square(c1->GetRect().GetHalf().x + c2->GetRect().GetHalf().x);
}

bool Physics2D::Collisions2D::CollisionTestPolygonToPolygon(Collider* c1, Collider* c2, FHitResult& hitResult)
{
    PolygonCollider* p1 = Cast<PolygonCollider>(c1);
    PolygonCollider* p2 = Cast<PolygonCollider>(c2);
    float depth;
    bool result = p1->polygon.Intersects(p2->polygon, depth, hitResult.ImpactNormal, hitResult.ImpactPoint);
    return result;
}
