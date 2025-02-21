/**
 * @file	SceneComponent.h
 * @brief	所有包含场景属性的组件的父类，赋予类对象场景意义
 * @author	Arty
 **/


#pragma once
#include"Components/ActorComponent.h"



/*----------------------------------
			  场景组件基类
  ----------------------------------*/
class SceneComponent : public ActorComponent
{
	FTransform transform_local;
	std::unordered_set<SceneComponent*>children;
	SceneComponent* parent = nullptr;//处理场景属性关系，便于增删

	void process_Destruct();

public:
	//设置所属组件
	void AttachTo(SceneComponent* par);

	//解除所属组件
	void DetachFrom(SceneComponent* par);

	//销毁组件
	virtual void Destruct() override;

	/** 获取场景属性（相对父对象坐标系）**/
	const FVector2D& GetLocalPosition() const { return transform_local.position; }
	float GetLocalRotation() const { return transform_local.rotation; }
	const FVector2D& GetLocalScale() const { return transform_local.scale; }
	const FTransform& GetLocalTransform() const { return transform_local; }

	/** 获取场景属性（世界绝对坐标系）**/
	FVector2D GetWorldPosition() const;
	float GetWorldRotation() const;
	FVector2D GetWorldScale() const;

	/** 设置场景属性（相对父对象坐标系）**/
	void SetLocalPosition(const FVector2D& pos) { transform_local.position = pos; }
	void SetLocalRotation(float angle) { transform_local.rotation = angle; }
	void SetLocalScale(const FVector2D& scale) { transform_local.scale = scale; }
	void SetLocalTransform(const FTransform& transform) { transform_local = transform; }

	/** 增加场景属性偏移量 **/
	void AddPosition(FVector2D pos) { transform_local.position += pos; }
	void AddRotation(float rot) { transform_local.rotation += rot; }
};
