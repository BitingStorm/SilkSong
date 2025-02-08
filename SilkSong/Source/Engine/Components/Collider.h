/**
 * @file	Collider.h
 * @brief	游戏场景中所有碰撞相关事件的处理
 * @author	Arty
 **/


#pragma once
#include"SceneComponent.h"
#include"Tools/CollisionManager.h"


/* 碰撞体形状 */
enum class ColliderShape:uint8 { Circle, Box };

/* 碰撞体模式 */
enum class CollisionMode:uint8 { None, Trigger, Collision };


class RigidBody;
class Controller;
class Collider;
class World;


/* 碰撞结果 */
struct HitResult
{
	FVector2D ImpactPoint;
	FVector2D ImpactNormal;
	Actor* HitObject;
	ActorComponent* HitComponent;

	HitResult() :ImpactPoint(0, 0), ImpactNormal(0, 0), HitObject(nullptr), HitComponent(nullptr) {}
	HitResult(const FVector2D& impactPoint, const FVector2D& impactNormal, Actor* hitObject, ActorComponent* hitComponent)
		:ImpactPoint(impactPoint), ImpactNormal(impactNormal), HitObject(hitObject), HitComponent(hitComponent) {}
};




/** 碰撞委托 **/
/* Collider* overlapComp, Collider* otherComp, Actor* otherActor */
DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(CollisionOverlapDelegate, Collider*, Collider*, Actor*)

/* Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const HitResult& hitResult */
DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(CollisionHitDelegate, Collider*, Collider*, Actor*, FVector2D, const HitResult&)





/*----------------------------------
			    碰撞器
  ----------------------------------*/
class Collider :public SceneComponent
{
	DEFINE_SUPER(SceneComponent)

	friend Controller;
	friend RigidBody;
	friend World;

public:
	Collider();
	virtual ~Collider();

	virtual void BeginPlay() override;
	virtual void Update(float deltaTime) override;
	virtual void Deactivate()override;

	//获取正在和该碰撞体发生碰撞的某一指定碰撞类型的所有碰撞体指针，以数组形式返回
	const std::vector<Actor*>& GetCollisions(CollisionType type);

	/** 碰撞层级 **/
	int GetLayer()const { return layer; }
	void SetLayer(int layer) { this->layer = layer; }

	/** 碰撞类型 **/
	CollisionType GetType()const { return type; }
	void SetType(CollisionType type) { this->type = type; }

	/** 碰撞标签 **/
	std::string GetTag()const { return collisionTag; }
	void SetTag(std::string tag) { collisionTag = tag; }

	//碰撞体形状
	ColliderShape GetShape()const { return shape; }

	/** 碰撞模式 **/
	void SetCollisonMode(CollisionMode mode);
	CollisionMode GetCollisonMode()const { return mode; }

	//是否与任何碰撞体无接触
	bool IsCollisionsEmpty()const { return collisions.size() == 0; }

	//绘制调试线
	virtual void DrawDebugLine() = 0;

	//设置物理材质
	void SetPhysicsMaterial(const FPhysicsMaterial& material) { this->material = material; }

	//获取矩形框
	FRect GetRect()const { return rect; }

	/** 碰撞事件 **/
	CollisionOverlapDelegate OnComponentBeginOverlap;
	CollisionOverlapDelegate OnComponentEndOverlap;
	CollisionOverlapDelegate OnComponentOverlap;
	CollisionHitDelegate OnComponentHit;
	CollisionHitDelegate OnComponentStay;

protected:
	FPhysicsMaterial material;

	ColliderShape shape{};

	//是否在鼠标所属世界坐标位置
	virtual bool IsMouseOver() = 0;

	FRect rect;//矩形框

private:
	int32 layer = 0;
	CollisionType type = CollisionType::Default;
	CollisionMode mode = CollisionMode::Trigger;
	std::string collisionTag;

	FIntVector2 point{ -1, -1 }, point_1{ -1, -1 };
	std::unordered_set<Collider*>collisions;
	std::vector<Actor*>aims;
	std::vector<Collider*>collisions_to_erase;

  
	//将碰撞体从世界彻底移除
	void Clear();

	//处理碰撞体之间的信息插入逻辑
	void Insert(Collider* another);

	//处理碰撞体之间的信息删除逻辑
	void Erase();


	bool CollisionJudge(Collider* another);

	/** 碰撞判断 **/
	static bool (*collisionJudgeMap[3])(Collider*, Collider*);
	static bool collisionJudgeCircleToCircle(Collider* c1, Collider* c2);
	static bool collisionJudgeCircleToBox(Collider* c1, Collider* c2);
	static bool collisionJudgeBoxToBox(Collider* c1, Collider* c2);
	
	HitResult CollisionHit(Collider* another);

	/** 碰撞信息 **/
	static HitResult (*collisionHitMap[3])(Collider*, Collider*);
	static HitResult collisionHitCircleToCircle(Collider* c1, Collider* c2);
	static HitResult collisionHitCircleToBox(Collider* c1, Collider* c2);
	static HitResult collisionHitBoxToBox(Collider* c1, Collider* c2);


	RigidBody* rigidAttached = nullptr;//附着的刚体
};


/*----------------------------------
			  圆形碰撞器
  ----------------------------------*/
class CircleCollider final :public Collider
{
public:
	CircleCollider() { shape = ColliderShape::Circle; }
	virtual void Update(float deltaTime)override;
	virtual void DrawDebugLine()override;
	virtual bool IsMouseOver()override;
	float GetRadius()const { return radius; }
	void SetRadius(float r);
private:
	float radius = 0;
	float radius_ini = 0;
};


/*----------------------------------
			  矩形碰撞器
  ----------------------------------*/
class BoxCollider final :public Collider
{
public:
	BoxCollider() { shape = ColliderShape::Box; }
	virtual void Update(float deltaTime)override;
	virtual void DrawDebugLine()override;
	virtual bool IsMouseOver()override;
	FVector2D GetSize()const { return size; }
	void SetSize(FVector2D size);
private:
	FVector2D size{};
	FVector2D size_ini{};
};