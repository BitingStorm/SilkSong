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
	Transform transform_local;
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
	const Vector2D& GetLocalPosition() const { return transform_local.position; }
	float GetLocalRotation() const { return transform_local.rotation; }
	const Vector2D& GetLocalScale() const { return transform_local.scale; }

	/** 获取场景属性（世界绝对坐标系）**/
	Vector2D GetWorldPosition() const;
	float GetWorldRotation() const;
	Vector2D GetWorldScale() const;

	/** 设置场景属性（相对父对象坐标系）**/
	void SetLocalPosition(const Vector2D& pos) { transform_local.position = pos; }
	void SetLocalRotation(float angle) { transform_local.rotation = angle; }
	void SetLocalScale(const Vector2D& scale) { transform_local.scale = scale; }

	/** 增加场景属性偏移量 **/
	void AddPosition(Vector2D pos) { transform_local.position += pos; }
	void AddRotation(float rot) { transform_local.rotation += rot; }
};
