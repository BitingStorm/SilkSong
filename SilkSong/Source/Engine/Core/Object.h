/**
 * @file	Object.h
 * @brief	游戏基础类定义，Object为绝大多数类的基类
 * @author	Arty
 **/

#pragma once
#include<iostream>
#include<string>



/*----------------------------------
			 基础对象类
  ----------------------------------*/
class Object 
{
	static int tag;

	std::string name;
public:
	Object() { tag++; name = typeid(*this).name() + std::string(" ") + std::to_string(tag); }
	virtual ~Object() { tag--; }

	//获取标签名
	std::string GetName()const { return name; }
	//设置标签名
	void SetName(const std::string& name) { this->name = name; }

	virtual void Update(float deltaTime) {}
	virtual void BeginPlay() {}

	//禁止用户自定义构造函数，而是使用该函数
	virtual void EndPlay() {}
};


/*----------------------------------
			 游戏关卡
  ----------------------------------*/
class Level :public Object
{
	friend class World;
	friend class GameplayStatics;

protected:
	class Controller* mainController = nullptr;

public:
	/* 务必把关卡初始化逻辑写进BeginPlay，禁止写入构造函数 */
	virtual void BeginPlay() {}
};


/*----------------------------------
			 全局游戏单例
  ----------------------------------*/
class GameInstance
{

};