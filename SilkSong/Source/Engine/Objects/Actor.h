/**
 * @file	Actor.h
 * @brief	场景对象基类定义，其本身与其派生类是游戏场景中的唯一存在形式
 * @author	Arty
 **/



#pragma once
#include"CoreMinimal.h"
#include"Tools/Timer.h"

class ActorComponent;
class SceneComponent;


/*----------------------------------
			 场景对象基类
  ----------------------------------*/
class Actor :public Object, public TimerHandler
{
	friend class RigidBody;

	std::unordered_set<Actor*>children;
	Actor* parent = nullptr;//处理场景属性关系 并且 便于增删

	std::unordered_set<ActorComponent*>components;
	std::unordered_set<ActorComponent*>::iterator components_iter;//管理组件的遍历以及增删

	bool bDestroyFlag = false;

protected:
	SceneComponent* const root = nullptr;//场景根组件，赋予场景属性

public:
	Actor();

	virtual ~Actor();

	virtual void Update(float deltaTime)override;

	virtual void BeginPlay()override;

	virtual void EndPlay()override;

	//获取所属场景对象
	Actor* GetOwner()const { return parent; }

	//设置场景根组件(如果不清楚底层关系，不建议使用，否则容易出错)
	void SetRootComponent(SceneComponent* newRoot);

	//设置所属对象
	void AttachTo(Actor* par);

	//解除所属对象
	void DetachFrom(Actor* par);


	//将组件注册到对象容器
	void RegisterComponent(ActorComponent* pCom);

	//从对象容器中注销组件
	void UnregisterComponent(ActorComponent* pCom);

	//销毁游戏对象
	void Destroy();

	/** 获取场景属性（相对父对象坐标系）**/
	const FVector2D& GetLocalPosition() const;
	float GetLocalRotation() const;
	const FVector2D& GetLocalScale() const;
	const FTransform& GetLocalTransform() const;

	/** 获取场景属性（世界绝对坐标系）**/
	FVector2D GetWorldPosition()const;
	float GetWorldRotation()const;
	FVector2D GetWorldScale()const;

	/** 设置场景属性（相对父对象坐标系）**/
	void SetLocalPosition(const FVector2D& pos);
	void SetLocalRotation(float angle);
	void SetLocalScale(const FVector2D& scale);
	void SetPositionAndRotation(const FVector2D& pos, float angle);
	void SetLocalTransform(const FTransform& transform);

	/** 增加场景属性偏移量 **/
	void AddPosition(FVector2D pos);
	void AddRotation(float rot);

	//绘制位置信息
	void DrawDebugPosition() const;

	//创建组件,绑定到对象,并注册到对象容器
	template<typename T>
	T* ConstructComponent()
	{
		T* pCom = new T;
		if (pCom && static_cast<ActorComponent*>(pCom))
		{
			pCom->SetOwner(this);
			RegisterComponent(pCom);
			return pCom;
		}
		delete pCom;
		return nullptr;
	}

	//通过类名获取组件
	template<typename T>
	T* GetComponentByClass()
	{
		for (auto& com : components)
		{
			if (T* pCom = Cast<T>(com))return pCom;
		}
		return nullptr;
	}

	//通过类名获取组件
	template<typename T>
	std::vector<T*> GetComponentsByClass()
	{
		std::vector<T*> result;
		result.reserve(components.size());
		for (auto& obj : components)
		{
			if (T* pObj = Cast<T>(obj))result.push_back(pObj);
		}
		return result;
	}

	//通过标签名获取组件
	ActorComponent* GetComponentByName(std::string tagName);

	//全局对象注册
	void RegisterDontDestroy();
};