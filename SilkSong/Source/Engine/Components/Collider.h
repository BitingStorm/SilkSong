/**
 * @file	Collider.h
 * @brief	��Ϸ������������ײ����¼��Ĵ���
 * @author	Arty
 **/


#pragma once
#include"SceneComponent.h"
#include"Tools/Delegate.h"
#include"Tools/CollisionManager.h"
#include"Core/World.h"


/* ��ײ����״ */
enum class ColliderShape:uint8 { Circle, Box };

/* ��ײ��ģʽ */
enum class CollisionMode:uint8 { None, Trigger, Collision };


class RigidBody;
class Controller;
class Collider;


/* ��ײ��� */
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




/** ��ײί�� **/
/* Collider* overlapComp, Collider* otherComp, Actor* otherActor */
DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(CollisionOverlapDelegate, Collider*, Collider*, Actor*)

/* Collider* hitComp, Collider* otherComp, Actor* otherActor, FVector2D normalImpulse, const HitResult& hitResult */
DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(CollisionHitDelegate, Collider*, Collider*, Actor*, FVector2D, const HitResult&)





/*----------------------------------
			    ��ײ��
  ----------------------------------*/
class Collider :public SceneComponent
{
	DEFINE_SUPER(SceneComponent)

	friend Controller;
	friend RigidBody;
	friend void World::ProcessColliders();

public:
	Collider():type(CollisionType::Default) { mainWorld.GameColliders.insert(this); }
	virtual ~Collider();

	virtual void BeginPlay() override;
	virtual void Update(float deltaTime) override;
	virtual void Deactivate()override;

	//��ȡ���ں͸���ײ�巢����ײ��ĳһָ����ײ���͵�������ײ��ָ�룬��������ʽ����
	const std::vector<Actor*>& GetCollisions(CollisionType type);

	/** ��ײ�㼶 **/
	int GetLayer()const { return layer; }
	void SetLayer(int layer) { this->layer = layer; }

	/** ��ײ���� **/
	CollisionType GetType()const { return type; }
	void SetType(CollisionType type) { this->type = type; }

	/** ��ײ��ǩ **/
	std::string GetTag()const { return collisionTag; }
	void SetTag(std::string tag) { collisionTag = tag; }

	//��ײ����״
	ColliderShape GetShape()const { return shape; }

	/** ��ײģʽ **/
	void SetCollisonMode(CollisionMode mode);
	CollisionMode GetCollisonMode()const { return mode; }

	//�Ƿ����κ���ײ���޽Ӵ�
	bool IsCollisionsEmpty()const { return collisions.size() == 0; }

	//���Ƶ�����
	virtual void DrawDebugLine() = 0;

	//�����������
	void SetPhysicsMaterial(const FPhysicsMaterial& material) { this->material = material; }

	//��ȡ���ο�
	FRect GetRect()const { return rect; }

	/** ��ײ�¼� **/
	CollisionOverlapDelegate OnComponentBeginOverlap;
	CollisionOverlapDelegate OnComponentEndOverlap;
	CollisionOverlapDelegate OnComponentOverlap;
	CollisionHitDelegate OnComponentHit;
	CollisionHitDelegate OnComponentStay;

protected:
	FPhysicsMaterial material;

	ColliderShape shape = ColliderShape::Circle;

	//�Ƿ������������������λ��
	virtual bool IsMouseOver() = 0;

	FRect rect;//���ο�

private:
	int layer = 0;
	CollisionType type;
	CollisionMode mode = CollisionMode::Trigger;
	std::string collisionTag;

	FPair point{ -1, -1 }, point_1{ -1, -1 };
	std::unordered_set<Collider*>collisions;
	std::vector<Actor*>aims;
	std::vector<Collider*>collisions_to_erase;

  
	//����ײ������糹���Ƴ�
	void Clear();

	//������ײ��֮�����Ϣ�����߼�
	void Insert(Collider* another);

	//������ײ��֮�����Ϣɾ���߼�
	void Erase();


	bool CollisionJudge(Collider* another);

	/** ��ײ�ж� **/
	static bool (*collisionJudgeMap[3])(Collider*, Collider*);
	static bool collisionJudgeCircleToCircle(Collider* c1, Collider* c2);
	static bool collisionJudgeCircleToBox(Collider* c1, Collider* c2);
	static bool collisionJudgeBoxToBox(Collider* c1, Collider* c2);
	
	HitResult CollisionHit(Collider* another);

	/** ��ײ��Ϣ **/
	static HitResult (*collisionHitMap[3])(Collider*, Collider*);
	static HitResult collisionHitCircleToCircle(Collider* c1, Collider* c2);
	static HitResult collisionHitCircleToBox(Collider* c1, Collider* c2);
	static HitResult collisionHitBoxToBox(Collider* c1, Collider* c2);


	RigidBody* rigidAttached = nullptr;//���ŵĸ���
};


/*----------------------------------
			  Բ����ײ��
  ----------------------------------*/
class CircleCollider final :public Collider
{
public:
	CircleCollider() { shape = ColliderShape::Circle; }
	virtual void Update(float deltaTime)override;
	virtual void DrawDebugLine()override;
	virtual bool IsMouseOver()override;
	float GetRadius()const { return radius; }
	void SetRadius(float r) { radius = r; radius_ini = r / sqrtf(GetWorldScale().x * GetWorldScale().y); }
private:
	float radius = 0;
	float radius_ini = 0;
};


/*----------------------------------
			  ������ײ��
  ----------------------------------*/
class BoxCollider final :public Collider
{
public:
	BoxCollider() { shape = ColliderShape::Box; }
	virtual void Update(float deltaTime)override;
	virtual void DrawDebugLine()override;
	virtual bool IsMouseOver()override;
	FVector2D GetSize()const { return size; }
	void SetSize(FVector2D size) { this->size = size; size_ini = size / GetWorldScale(); }

	//��ȡ�ص����ο�ߣ��贫���Ѿ�ȷ��������ײ��������ײ��
	static FVector2D GetOverlapRect(const FRect& r1, const FRect& r2);
private:
	FVector2D size = FVector2D(0, 0);
	FVector2D size_ini = FVector2D(0, 0);
};