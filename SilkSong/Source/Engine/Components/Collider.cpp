#include "Collider.h"
#include "Camera.h"
#include "RigidBody.h"
#include "Objects/Controller.h"
#include "Tools/Math.h"
#include "GameplayStatics.h"
#include "easyx.h"


bool (*Collider::collisionJudgeMap[3])(Collider*, Collider*) = 
{ &Collider::collisionJudgeCircleToCircle,Collider::collisionJudgeCircleToBox,Collider::collisionJudgeBoxToBox };

HitResult (*Collider::collisionHitMap[3])(Collider*, Collider*) =
{ &Collider::collisionHitCircleToCircle,Collider::collisionHitCircleToBox,Collider::collisionHitBoxToBox };



Collider::~Collider()
{
    if (rigidAttached)rigidAttached->colliders.erase(this);
    mainWorld.GameColliders.erase(this);Clear();
}

void Collider::BeginPlay()
{
    Super::BeginPlay();

    rigidAttached = pOwner->GetComponentByClass<RigidBody>();
    if(rigidAttached)rigidAttached->colliders.insert(this);
}

void Collider::Update(float deltaTime)
{
    Super::Update(deltaTime);

    if (mode == CollisionMode::None || !bIsEnabled)return;

    for (auto& another : collisions)
    {
        if (another->mode == CollisionMode::Collision && this->mode == CollisionMode::Collision)
        {
            HitResult hitResult = this->CollisionHit(another);
            OnComponentStay.BroadCast(this, another, another->pOwner, -hitResult.ImpactNormal, hitResult);
            another->OnComponentStay.BroadCast(another, this, pOwner, hitResult.ImpactNormal, { hitResult.ImpactPoint,-hitResult.ImpactNormal,pOwner,this });
        }
        else
        {
            OnComponentOverlap.BroadCast(this, another, another->pOwner);  another->OnComponentOverlap.BroadCast(another, this, pOwner);
        }
    }

    Vector2D half = GetRect().GetSize()*0.5f;

    Vector2D pos = GetWorldPosition() - half;
    Pair newPoint(Math::Clamp(int(pos.x + 2000) / 400, 0, 9), Math::Clamp(int(pos.y) / 200, 0, 5));
    pos += half * 2;
    Pair newPoint_1(Math::Clamp(int(pos.x + 2000) / 400, 0, 9), Math::Clamp(int(pos.y) / 200, 0, 5));
    if (newPoint == point && newPoint_1 == point_1)return;

    //碰撞区块信息更新
    if (point != Pair(-1, -1))for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.ColliderZones[i][j].erase(this);
    point = newPoint, point_1 = newPoint_1;
    for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.ColliderZones[i][j].insert(this);
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

void Collider::SetCollisonMode(CollisionMode mode)
{
    if (mode == CollisionMode::None && this->mode != CollisionMode::None)
    {
        mainWorld.GameColliders_to_clear.insert(this);
    }
    this->mode = mode;
}

void Collider::Clear()
{
    for (auto& another : collisions) 
    {
        another->collisions.erase(this);
        OnComponentEndOverlap.BroadCast(this,another,another->pOwner);  another->OnComponentEndOverlap.BroadCast(another,this,pOwner);
    }
    collisions.clear();
    if (point != Pair(-1, -1))
    {
        for (int i = point.x; i <= point_1.x; ++i)for (int j = point.y; j <= point_1.y; ++j)mainWorld.ColliderZones[i][j].erase(this);
    }
    point = { -1, -1 }, point_1 = { -1, -1 };
}

void Collider::Insert(Collider* another)
{
    if (mainWorld.collisionManager->FindMapping(this->type, another->type)
        && collisions.find(another) == collisions.end() && CollisionJudge(another)) 
    {
        collisions.insert(another); another->collisions.insert(this);
        if (another->mode == CollisionMode::Collision && this->mode == CollisionMode::Collision)
        {
            HitResult hitResult = this->CollisionHit(another);
            OnComponentHit.BroadCast(this, another, another->pOwner,-hitResult.ImpactNormal,hitResult);
            another->OnComponentHit.BroadCast(another, this, pOwner, hitResult.ImpactNormal, {hitResult.ImpactPoint,-hitResult.ImpactNormal,pOwner,this});
            if (rigidAttached)
            {
                rigidAttached->RestrictVelocity(-hitResult.ImpactNormal, PhysicsMaterial::Combine(this->material, another->material), another->rigidAttached);
            }
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
        if (!CollisionJudge(another)) 
        {
            another->collisions.erase(this); collisions_to_erase.push_back(another);
            OnComponentEndOverlap.BroadCast(this,another,another->pOwner); another->OnComponentEndOverlap.BroadCast(another,this,pOwner);
        }
    }
    for (auto& another : collisions_to_erase) collisions.erase(another);
}


bool Collider::CollisionJudge(Collider* another)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionJudgeMap[shape1*shape1 + shape2*shape2](this, another);
}

bool Collider::collisionJudgeCircleToCircle(Collider* c1, Collider* c2)
{
    CircleCollider* circle1 = Cast<CircleCollider>(c1);
    CircleCollider* circle2 = Cast<CircleCollider>(c2);
    return Vector2D::Distance(circle1->GetWorldPosition(), circle2->GetWorldPosition()) <= circle1->GetRadius() + circle2->GetRadius();
}

bool Collider::collisionJudgeCircleToBox(Collider* c1, Collider* c2)
{
    CircleCollider* circle;BoxCollider* box;
    if (c1->GetShape() == ColliderShape::Circle) 
    { 
        circle = Cast<CircleCollider>(c1),box = Cast<BoxCollider>(c2);
    }
    else
    {
        circle = Cast<CircleCollider>(c2),box = Cast<BoxCollider>(c1);
    }
    float radius = circle->GetRadius();Vector2D pos = circle->GetWorldPosition();
    Rect rect = box->GetRect();

    if (pos.x <= rect.right && pos.x >= rect.left && pos.y <= rect.top && pos.y >= rect.bottom)
        return true;
    else
    {
        if (pos.x < rect.left)
        {
            if (pos.y > rect.top)return Vector2D::Distance(pos, { rect.left,rect.top }) <= radius;
            else if (pos.y < rect.bottom)return Vector2D::Distance(pos, { rect.left,rect.bottom }) <= radius;
            else return rect.left - pos.x <= radius;
        }
        else if (pos.x > rect.right)
        {
            if (pos.y > rect.top)return Vector2D::Distance(pos, { rect.right,rect.top }) <= radius;
            else if (pos.y < rect.bottom)return Vector2D::Distance(pos, { rect.right,rect.bottom }) <= radius;
            else return pos.x - rect.right <= radius;
        }
        else
        {
            if (pos.y > rect.top)return pos.y - rect.top <= radius;
            else return rect.bottom - pos.y <= radius;
        }
    }
}

bool Collider::collisionJudgeBoxToBox(Collider* c1, Collider* c2)
{
    BoxCollider* box1 = Cast<BoxCollider>(c1);
    BoxCollider* box2 = Cast<BoxCollider>(c2);
    Vector2D pos1 = box1->GetWorldPosition() - box1->GetSize() / 2;
    Vector2D pos2 = box2->GetWorldPosition() - box2->GetSize() / 2;
    return (pos1.x < pos2.x + box2->GetSize().x && pos1.x + box1->GetSize().x > pos2.x &&
        pos1.y < pos2.y + box2->GetSize().y && pos1.y + box1->GetSize().y > pos2.y);
}

HitResult Collider::CollisionHit(Collider* another)
{
    int shape1 = int(this->shape), shape2 = int(another->shape);
    return collisionHitMap[shape1 * shape1 + shape2 * shape2](this, another);
}

HitResult Collider::collisionHitCircleToCircle(Collider* c1, Collider* c2)
{
    CircleCollider* circle1 = Cast<CircleCollider>(c1);
    CircleCollider* circle2 = Cast<CircleCollider>(c2);
    Vector2D impactNormal = (circle2->GetWorldPosition()-circle1->GetWorldPosition()).Normalize();
    Vector2D impactPoint = circle1->GetWorldPosition() + impactNormal * circle1->GetRadius();
    return HitResult(impactPoint, impactNormal,circle2->pOwner,circle2);
}

HitResult Collider::collisionHitCircleToBox(Collider* c1, Collider* c2)
{
    CircleCollider* circle; BoxCollider* box;
    if (c1->GetShape() == ColliderShape::Circle)
    {
        circle = Cast<CircleCollider>(c1), box = Cast<BoxCollider>(c2);
    }
    else
    {
        circle = Cast<CircleCollider>(c2), box = Cast<BoxCollider>(c1);
    }
    float radius = circle->GetRadius(); Vector2D pos = circle->GetWorldPosition();
    Rect rect = box->GetRect();

    Vector2D impactNormal;
    Vector2D impactPoint;

    if (pos.x <= rect.right && pos.x >= rect.left && pos.y <= rect.top && pos.y >= rect.bottom)
    {
        impactPoint = pos;
        impactNormal = (c2->GetWorldPosition() - c1->GetWorldPosition()).Normalize();
    }
    else
    {
        if (pos.x < rect.left)
        {
            if (pos.y > rect.top){impactPoint = { rect.left,rect.top };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize();}
            else if (pos.y < rect.bottom){impactPoint = { rect.left,rect.bottom };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize();}
            else{impactPoint = { rect.left,pos.y };  impactNormal = {1,0};}
        }
        else if (pos.x > rect.right)
        {
            if (pos.y > rect.top){impactPoint = { rect.right,rect.top };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize();}
            else if (pos.y < rect.bottom){impactPoint = { rect.right,rect.bottom };  impactNormal = (impactPoint - circle->GetWorldPosition()).Normalize();}
            else{impactPoint = { rect.right,pos.y }; impactNormal = { -1,0 };}
        }
        else
        {
            if (pos.y > rect.top){impactPoint = { pos.x,rect.top }; impactNormal = { 0,-1 };}
            else{impactPoint = { pos.x,rect.bottom }; impactNormal = { 0,1 };}
        }
    }
    return HitResult(impactPoint, impactNormal * (c1==circle?1.f:-1.f), c2->pOwner, c2);
}

HitResult Collider::collisionHitBoxToBox(Collider* c1, Collider* c2)
{
    BoxCollider* box1 = Cast<BoxCollider>(c1);
    BoxCollider* box2 = Cast<BoxCollider>(c2);
    Rect r1 = box1->GetRect(), r2 = box2->GetRect();
    Vector2D overlapRect = BoxCollider::GetOverlapRect(r1, r2);
    Vector2D impactNormal;
    if (overlapRect.x >= overlapRect.y)
    {
        box1->GetWorldPosition().y - box2->GetWorldPosition().y > 0 ? impactNormal = { 0,-1 } : impactNormal = { 0,1 };
    }
    else
    {
        box1->GetWorldPosition().x - box2->GetWorldPosition().x > 0 ? impactNormal = { -1,0 } : impactNormal = { 1,0 };
    }

    //计算重叠区域中心点
    float centerX = max(r1.left, r2.left) + overlapRect.x / 2;
    float centerY = max(r1.bottom, r2.bottom) + overlapRect.y / 2;

    return HitResult({centerX, centerY}, impactNormal, box2->pOwner, box2);
}







void CircleCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);

    radius = radius_ini * sqrtf(std::abs(GetWorldScale().x * GetWorldScale().y));
    Vector2D pos = GetWorldPosition();
    rect = { pos.x - radius, pos.y + radius, pos.x + radius, pos.y - radius };
}

void CircleCollider::DrawDebugLine()
{
    if (GetCollisonMode() == CollisionMode::None)return;
    setlinecolor(GREEN);
    Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition())
        * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength() + Vector2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    circle((int)pos.x,(int)pos.y,int(radius * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength()));
}

bool CircleCollider::IsMouseOver()
{
    return Vector2D::Distance(GetWorldPosition(), GameplayStatics::GetController()->GetCursorPosition()) <= radius;
}








void BoxCollider::Update(float deltaTime)
{
    Collider::Update(deltaTime);

    size = size_ini * GetWorldScale(); size.x = std::abs(size.x); size.y = std::abs(size.y);
    Vector2D pos = GetWorldPosition();
    rect = { pos.x - size.x / 2 ,pos.y + size.y / 2 ,pos.x + size.x / 2 ,pos.y - size.y / 2 };
}

void BoxCollider::DrawDebugLine()
{
    if (GetCollisonMode() == CollisionMode::None)return;
    setlinecolor(GREEN);
    Vector2D pos = (GetWorldPosition() - mainWorld.mainCamera->GetVirtualPosition())
        * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength() + Vector2D(WIN_WIDTH / 2, WIN_HEIGHT / 2);
    Vector2D si = this->size * 20.f / mainWorld.mainCamera->GetVirtualSpringArmLength();
    float left = pos.x - si.x / 2, right = pos.x + si.x / 2,top = pos.y + si.y / 2, bottom = pos.y - si.y / 2;
    rectangle((int)left,(int)top,(int)right,(int)bottom);
}

bool BoxCollider::IsMouseOver()
{
    Vector2D pos = GetWorldPosition(),pos_m = GameplayStatics::GetController()->GetCursorPosition();
    return pos_m.x <= pos.x + size.x / 2 && pos_m.x >= pos.x - size.x / 2
        && pos_m.y <= pos.y + size.y / 2 && pos_m.y >= pos.y - size.y / 2;
}

Vector2D BoxCollider::GetOverlapRect(const Rect& r1, const Rect& r2)
{
    return { min(r1.right, r2.right) - max(r1.left, r2.left), min(r1.top, r2.top) - max(r1.bottom, r2.bottom) };
}


