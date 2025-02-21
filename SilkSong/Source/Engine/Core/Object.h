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
	static int number;

	std::string name;

public:
	Object() { number++; InitName(std::string(typeid(*this).name()).substr(6)); }
	virtual ~Object() { number--; }

	//初始化标签名
	void InitName(const std::string& name) 
	{ 
		this->name = name + std::string("_") + std::to_string(number);
	}

	//获取标签名
	std::string GetName()const { return name; }

	//设置标签名
	void SetName(const std::string& name) { this->name = name; }

	virtual void Update(float deltaTime) {}

	virtual void BeginPlay() {}

	//禁止用户自定义析构函数，而是使用该函数
	virtual void EndPlay() {}
};



/*----------------------------------
			 全局游戏单例
  ----------------------------------*/
class GameInstance
{
public:
	GameInstance();
};