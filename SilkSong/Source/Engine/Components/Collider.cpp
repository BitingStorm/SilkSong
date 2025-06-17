#include "Collider.h"
#include "Camera.h"
#include "RigidBody.h"
#include "Objects/Controller.h"
#include "GameplayStatics.h"
#include "easyx.h"


bool (*Collider::collisionJudgeMap[3])(Collider*, Collider*) =
{ &Collider::collisionJudgeCircleToCircle,Collider::collisionJudgeCircleToBox,Collider::collisionJudgeBoxToBox };

FHitResult(*Collider::collisionHitMap[3])(Collider*, Collider*) =
{ &Collider::collisionHitCircleToCircle,Collider::collisionHitCircleToBox,Collider::collisionHitBoxToBox };

void (*Collider::collisionAdjustMap[3])(Collider*, Collider*, const FHitResult&) =
{ &Collider::collisionAdjustCircleToCircle,Collider::collisionAdjustCircleToBox,Collider::collisionAdjustBoxToBox };


Collider::Collider()
{
    mainWorld.GameColliders.insert(this);
}

Collider::~Collider()
{
    if (rigidAttached)rigidAttached->colliders.erase(this);
    mainWorld.GameColliders.erase(this); Clear();
}

void Collider::BeginPlay()
{
    Super::BeginPlay();

    rigidAttached = pOwner->GetComponentByClass<RigidBody>();
    if (rigidAttached)rigidAttached->colliders.insert(this);
}

void Collider::Update(float deltaTime)
{
    Super::Update(deltaTime);

    if (mode == CollisionMode::None || !bIsEnabled)return;

    for (auto& another : collisions)
    {
        if (another->mode == CollisionMode::Collision && this->mode == CollisionMode::Collision)
        {
            FHitResult hitResult = this->CollisionHit(another);
            OnComponentStay.BroadCast(this, another, another->pOwner, -hitResult.ImpactNormal, hitResult);
            another->OnComponentStay.BroadCast(another, this, pOwner, hitResult.ImpactNormal, { hitResult.ImpactPoint,-hitResult.ImpactNormal,pOwner,this });
            CollisionAdjust(another, hitResult);
        }
        else
        {
            OnComponentOverlap.BroadCast(this, another, another->pOwner);  another->OnComponentOverlap.BroadCast(another, this, pOwner);
        }
    }
    ColliderZoneTick();
}

void Collider::Deactivate()
{
    Super::Deactivate();
    mainWorld.GameColliders_to_clear.insert(this);
}


const std::vector<Actor*>& Collider::GetCollisions(CollisionType type)
{
    aims.clear();
    for (auto& it : collisions)
    {
        if (it->type == type)
        {
            aims.push_back(it->pOwner);
        }
    }
    return aims;
}

void Collider::SetType(CollisionType type)
{
    this->type = type;
    layerMask = mainWorld.collisionManager->FindMapping(type);
}

void Collider::SetCollisionResponseToType(CollisionType aimType, bool enable)
{
    enable ? layerMask |= (1 << int(aimType)) : layerMask &= ~(1 << int(aimType));
}

void Collider::SetCollisonMode(CollisionMode mode)
{
    if (mode == CollisionMode::None && this->mode != CollisionMode::None)
    {
        mainWorld.GameColliders_to_clear.insert(this);
    }
    this->mode = mode;
}

bool Collider::IsKinematics() const
{
    return rigidAttached && rigidAttached->bMoveable;
}

void Collider::RegisterDontDestroy()
{
    mainWorld.OverallColliders.insert(this);
}

void Collider::ColliderZoneTick()
{
    if (mode == CollisionMode::None || !bIsEnabled)return;

    FVector2D pos = GetRect().min;
    FIntVector2 newPoint(FMath::Clamp(int(pos.x + 2000) / 400, 0, 9), FMath::Clamp(int(pos.y) / 200, 0, 5));
    pos = GetRect().max;
    FIntVector2 newPoint_1(FMath::Clamp(int(pos.x + 2000) / 400, 0, 9), FMath::Clamp(int(pos.y) / 200, 0, 5));
    if (newPoint == point && newPoint_1 == point_1)return;

    //碰撞区块信息更新
    if (point != FIntVector2(-1, -1))for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.ColliderZones[i][j].erase(this);
    point = newPoint, point_1 = newPoint_1;
    for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.ColliderZones[i][j].insert(this);
}

void Collider::Clear()
{
    for (auto& another : collisions)
    {
        another->collisions.erase(this);
        OnComponentEndOverlap.BroadCast(this, another, another->pOwner);  another->OnComponentEndOverlap.BroadCast(another, this, pOwner);
    }
    collisions.clear();
    if (point != FIntVector2(-1, -1))
    {
        for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.ColliderZones[i][j].erase(this);
    }
    point = { -1, -1 }, point_1 = { -1, -1 };
}

void Collider::Insert(Collider* another)
{
    if (mainWorld.collisionManager->LayerMaskJudge(this->layerMask, another->type)
        && mainWorld.collisionManager->LayerMaskJudge(another->layerMask, this->type)
        && collisions.find(another) == collisions.end() && CollisionJudge(another))
    {
        collisions.insert(another); another->collisions.insert(this);
        if (another->mode == CollisionMode::Collision && this->mode == CollisionMode::Collision)
        {
            FHitResult hitResult = CollisionHit(another);
            OnComponentHit.BroadCast(this, another, another->pOwner, -hitResult.ImpactNormal, hitResult);
            another->OnComponentHit.BroadCast(another, this, pOwner, hitResult.ImpactNormal, { hitResult.ImpactPoint,-hitResult.ImpactNormal,pOwner,this });
            
            if (rigidAttached)
            {
                rigidAttached->RestrictVelocity(-hitResult.ImpactNormal, FPhysicsMaterial::Combine(this->material, another->material), another->rigidAttached);
            }
            else if (another->rigidAttached)
            {
                another->rigidAttached->RestrictVelocity(hitResult.ImpactNormal, FPhysicsMaterial::Combine(this->material, another->material), rigidAttached);
            }
            
            CollisionAdjust(another, hitResult);
        }
        else
        {
            OnComponentBeginOverlap.BroadCast(this, another, another->pOwner); another->OnComponentBeginOverlap.BroadCast(another, this, pOwner);
        }
    }
}

void Collider::Erase()
{
    collisions_to_erase.clear();
    for (auto& another : collisions)
    {
        if (!CollisionJudge(another) || !mainWorld.collisionManager->LayerMaskJudge(this->layerMask, another->type))
        {
            another->collisions.erase(this); collisions_to_erase.push_back(another);
            OnComponentEndOverlap.BroadCast(this, another, another->pOwner);  another->OnComponentEndOverlap.BroadCast(another, this, pOwner);
        }
    }
    for (auto& another : collisions_to_erase) collisions.erase(another);
}

bool Collider::CollisionJudge(Collider* another)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionJudgeMap[shape1 * shape1 + shape2 * shape2](this, another);
}

bool Collider::collisionJudgeCircleToCircle(Collider* c1, Collider* c2)
{
    return FVector2D::DistSquared(c1->GetWorldPosition(), c2->GetWorldPosition()) 
        <= FMath::Square(c1->GetRect().GetHalf().x + c2->GetRect().GetHalf().x);
}

bool Collider::collisionJudgeCircleToBox(Collider* c1, Collider* c2)
{
    Collider* circle; Collider* box;
    if (c1->GetShape() == ColliderShape::Circle)
    {
        circle = c1, box = c2;
    }
    else
    {
        circle = c2, box = c1;
    }
    float radius = circle->GetRect().GetHalf().x;
    FVector2D pos = circle->GetWorldPosition();
    FRect rect = box->GetRect();

    if (rect.IsInsideOrOn(pos))
    {
        return true;
    }
    else
    {
        if (pos.x < rect.min.x)
        {
            if (pos.y > rect.max.y)return FVector2D::DistSquared(pos, { rect.min.x,rect.max.y }) <= radius * radius;
            else if (pos.y < rect.min.y)return FVector2D::DistSquared(pos, { rect.min.x,rect.min.y }) <= radius * radius;
            else return rect.min.x - pos.x <= radius;
        }
        else if (pos.x > rect.max.x)
        {
            if (pos.y > rect.max.y)return FVector2D::DistSquared(pos, { rect.max.x,rect.max.y }) <= radius * radius;
            else if (pos.y < rect.min.y)return FVector2D::DistSquared(pos, { rect.max.x,rect.min.y }) <= radius * radius;
            else return pos.x - rect.max.x <= radius;
        }
        else
        {
            if (pos.y > rect.max.y)return pos.y - rect.max.y <= radius;
            else return rect.min.y - pos.y <= radius;
        }
    }
}

bool Collider::collisionJudgeBoxToBox(Collider* c1, Collider* c2)
{
    return c1->GetRect().Intersects(c2->GetRect());
}

FHitResult Collider::CollisionHit(Collider* another)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionHitMap[shape1 * shape1 + shape2 * shape2](this, another);
}

FHitResult Collider::collisionHitCircleToCircle(Collider* c1, Collider* c2)
{
    FVector2D impactNormal = (c2->GetWorldPosition() - c1->GetWorldPosition()).GetSafeNormal();
    FVector2D impactPoint = c1->GetWorldPosition() + impactNormal * c1->GetRect().GetHalf().x;
    return FHitResult(impactPoint, impactNormal, c2->pOwner, c2);
}

FHitResult Collider::collisionHitCircleToBox(Collider* c1, Collider* c2)
{
    Collider* circle; Collider* box;
    if (c1->GetShape() == ColliderShape::Circle)
    {
        circle = c1, box = c2;
    }
    else
    {
        circle = c2, box = c1;
    }
    float radius = circle->GetRect().GetHalf().x; 
    FVector2D pos = circle->GetWorldPosition();
    FRect rect = box->GetRect();

    FVector2D impactNormal;
    FVector2D impactPoint;

    if (rect.IsInsideOrOn(pos))
    {
        impactPoint = pos;
        impactNormal = (c2->GetWorldPosition() - c1->GetWorldPosition()).GetSafeNormal();
    }
    else
    {
        if (pos.x < rect.min.x)
        {
            if (pos.y > rect.max.y) { impactPoint = { rect.min.x,rect.max.y };  impactNormal = (impactPoint - circle->GetWorldPosition()).GetSafeNormal(); }
            else if (pos.y < rect.min.y) { impactPoint = { rect.min.x,rect.min.y };  impactNormal = (impactPoint - circle->GetWorldPosition()).GetSafeNormal(); }
            else { impactPoint = { rect.min.x,pos.y };  impactNormal = { 1,0 }; }
        }
        else if (pos.x > rect.max.x)
        {
            if (pos.y > rect.max.y) { impactPoint = { rect.max.x,rect.max.y };  impactNormal = (impactPoint - circle->GetWorldPosition()).GetSafeNormal(); }
            else if (pos.y < rect.min.y) { impactPoint = { rect.max.x,rect.min.y };  impactNormal = (impactPoint - circle->GetWorldPosition()).GetSafeNormal(); }
            else { impactPoint = { rect.max.x,pos.y }; impactNormal = { -1,0 }; }
        }
        else
        {
            if (pos.y > rect.max.y) { impactPoint = { pos.x,rect.max.y }; impactNormal = { 0,-1 }; }
            else { impactPoint = { pos.x,rect.min.y }; impactNormal = { 0,1 }; }
        }
    }
    return FHitResult(impactPoint, impactNormal * (c1 == circle ? 1.f : -1.f), c2->pOwner, c2);
}

FHitResult Collider::collisionHitBoxToBox(Collider* c1, Collider* c2)
{
    FRect overlapRect = c1->GetRect().Overlaps(c2->GetRect());
    FVector2D impactNormal;
    if (overlapRect.GetSize().x >= overlapRect.GetSize().y)
    {
        c1->GetWorldPosition().y - c2->GetWorldPosition().y > 0 ? impactNormal = { 0,-1 } : impactNormal = { 0,1 };
    }
    else
    {
        c1->GetWorldPosition().x - c2->GetWorldPosition().x > 0 ? impactNormal = { -1,0 } : impactNormal = { 1,0 };
    }
    return FHitResult(overlapRect.GetCenter(), impactNormal, c2->pOwner, c2);
}

void Collider::CollisionAdjust(Collider* another, const FHitResult& hitResult)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionAdjustMap[shape1 * shape1 + shape2 * shape2](this, another, hitResult);
}

void Collider::collisionAdjustCircleToCircle(Collider* c1, Collider* c2, const FHitResult& hitResult)
{
    FVector2D impactNormal = hitResult.ImpactNormal;

    float separationDistance = c1->GetRect().GetHalf().x + c2->GetRect().GetHalf().x - FVector2D::Distance(c1->GetWorldPosition(), c2->GetWorldPosition());
    separationDistance = separationDistance - AE_KINDA_SMALL_NUMBER;
    if (separationDistance <= 0)
    {
        return;
    }

    if (c1->IsKinematics() && c2->IsKinematics())
    {
        c1->GetOwner()->AddPosition(-impactNormal * separationDistance * 0.5f);
        c2->GetOwner()->AddPosition(impactNormal * separationDistance * 0.5f);
    }
    else if (c1->IsKinematics())
    {
        c1->GetOwner()->AddPosition(-impactNormal * separationDistance);
    }
    else if (c2->IsKinematics())
    {
        c2->GetOwner()->AddPosition(impactNormal * separationDistance);
    }
}

void Collider::collisionAdjustCircleToBox(Collider* c1, Collider* c2, const FHitResult& hitResult)
{
    FVector2D impactNormal = hitResult.ImpactNormal;
    Collider* circle; Collider* box;
    if (c1->GetShape() == ColliderShape::Circle)
    {
        circle = c1, box = c2;
    }
    else
    {
        circle = c2, box = c1;
    }
    float separationDistance = circle->GetRect().GetHalf().x - FVector2D::Distance(hitResult.ImpactPoint, circle->GetWorldPosition());
    separationDistance = separationDistance - AE_KINDA_SMALL_NUMBER;
    if (separationDistance <= 0)
    {
        return;
    }

    if (c1->IsKinematics() && c2->IsKinematics())
    {
        if (c1->rigidAttached->bMoveable)c1->GetOwner()->AddPosition(-impactNormal * separationDistance * 0.5f);
        if (c2->rigidAttached->bMoveable)c2->GetOwner()->AddPosition(impactNormal * separationDistance * 0.5f);
    }
    else if (c1->IsKinematics())
    {
        c1->GetOwner()->AddPosition(-impactNormal * separationDistance);
    }
    else if (c2->IsKinematics())
    {
        c2->GetOwner()->AddPosition(impactNormal * separationDistance);
    }
}

void Collider::collisionAdjustBoxToBox(Collider* c1, Collider* c2, const FHitResult& hitResult)
{
    FRect overlapRect = c1->GetRect().Overlaps(c2->GetRect());
    FVector2D impactNormal = hitResult.ImpactNormal;

    float separationDistance = overlapRect.GetSize().x * FMath::Abs(impactNormal.x) + overlapRect.GetSize().y * FMath::Abs(impactNormal.y);
    separationDistance = separationDistance - AE_KINDA_SMALL_NUMBER;
    if (separationDistance <= 0)
    {
        return;
    }

    if (c1->IsKinematics() && c2->IsKinematics())
    {
        c1->GetOwner()->AddPosition(-impactNormal * separationDistance * 0.5f);
        c2->GetOwner()->AddPosition(impactNormal * separationDistance * 0.5f);
    }
    else if(c1->IsKinematics())
    {
        c1->GetOwner()->AddPosition(-impactNormal * separationDistance);
    }
    else if (c2->IsKinematics())
    {
        c2->GetOwner()->AddPosition(impactNormal * separationDistance);
    }
}







void CircleCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);

    radius = radius_ini * FMath::Sqrt(FMath::Abs(GetWorldScale().x * GetWorldScale().y));
}

void CircleCollider::DrawDebugLine()
{
    if (GetCollisonMode() == CollisionMode::None)return;
    setlinecolor(GREEN);
    FVector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition())
        * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
    circle((int)pos.x, (int)pos.y, int(radius * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()));
}

bool CircleCollider::IsMouseOver()
{
    return FVector2D::Distance(GetWorldPosition(), GameplayStatics::GetController()->GetCursorPosition()) <= radius;
}

void CircleCollider::SetRadius(float r)
{
    radius = FMath::Abs(r); 
    radius_ini = radius * FMath::InvSqrt(FMath::Abs(GetWorldScale().x * GetWorldScale().y));
}








void BoxCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);

    size = size_ini * GetWorldScale();
    size.MakeAbs();
}

void BoxCollider::DrawDebugLine()
{
    if (GetCollisonMode() == CollisionMode::None)return;
    setlinecolor(GREEN);
    FVector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition())
        * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
    FVector2D si = size * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength();
    float left = pos.x - si.x * 0.5f, right = pos.x + si.x * 0.5f, top = pos.y + si.y * 0.5f, bottom = pos.y - si.y * 0.5f;
    rectangle((int)left, (int)top, (int)right, (int)bottom);
}

bool BoxCollider::IsMouseOver()
{
    return GetRect().IsInside(GameplayStatics::GetController()->GetCursorPosition());
}

void BoxCollider::SetSize(FVector2D size)
{
    this->size = size.GetAbs(); 
    size_ini = size / GetWorldScale();
    size_ini.MakeAbs();
}







void PolygonCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);
}

void PolygonCollider::DrawDebugLine()
{
    if (GetCollisonMode() == CollisionMode::None || !polygon.bIsValid)return;
    setlinecolor(GREEN);
    size_t n = polygon.vertices.size();
    /*for (int i = 0, j = n - 1; i < n; j = i++)
    {
        line(polygon.vertices[i].x, polygon.vertices[i].y, polygon.vertices[j].x, polygon.vertices[j].y);
    }*/
}

bool PolygonCollider::IsMouseOver()
{
    return polygon.IsInside(GameplayStatics::GetController()->GetCursorPosition());
}

void PolygonCollider::SetVertices(std::vector<FVector2D> vertices)
{

}
