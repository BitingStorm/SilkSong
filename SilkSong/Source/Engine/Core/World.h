/**
 * @file	World.h
 * @brief	游戏世界类定义，包含所有基础Gameplay处理逻辑
 * @author	Arty
 **/



#pragma once
#include"CoreMinimal.h"
#include"Objects/Actor.h"
#include"UI/UserInterface.h"
#include <mutex>


class Engine;
class LevelManager;
class ResourceManager;
class CollisionManager;
class GameplayStatics;
class Actor;
class Timer;
class LayerInterface;
class ImageInterface;
class UserInterface;
class Widget;
class Button;
class Bar;
class Sector;
class Camera;
class SpriteRenderer;
class Animation;
class ParticleSystem;
class Collider;
class CircleCollider;
class BoxCollider;
class Controller;
class AudioPlayer;
class VideoPlayer;
class TimerHandler;
class Level;
class RigidBody;


namespace ArtyEngine
{
	/* 渲染图层排序规则 */
	struct LayerSort
	{
		bool operator()(const LayerInterface* a, const LayerInterface* b) const;
	};

	/* 碰撞图层排序规则 */
	struct ColliderSort
	{
		bool operator()(const Collider* a, const Collider* b) const;
	};
}

/*----------------------------------
			   游戏世界
  ----------------------------------*/
class World final
{
	/** 友元声明 **/
	friend Engine;
	friend GameplayStatics;
	friend GameInstance;
	friend Timer;
	friend LayerInterface;
	friend void Actor::Destroy();
	friend void Actor::DrawDebugPosition()const;
	friend ImageInterface;
	friend Camera;
	friend SpriteRenderer;
	friend Animation;
	friend ParticleSystem;
	friend Collider;
	friend RigidBody;
	friend CircleCollider;
	friend BoxCollider;
	friend Controller;
	friend void UserInterface::RemoveFromViewport();
	friend void UserInterface::RegisterDontDestroy();
	friend Widget;
	friend Button;
	friend Bar;
	friend Sector;
	friend LevelManager;
	friend AudioPlayer;
	friend VideoPlayer;
	friend TimerHandler;
	friend Level;

	std::mutex updateMutex;//互斥锁

	bool videoLock = false;//视频锁

	ResourceManager* resourcePool = nullptr;//资源池

	CollisionManager* collisionManager = nullptr;//碰撞管理器

	LevelManager* levelManager = nullptr;//地图管理器

	Timer* FPSClock = nullptr;//帧计时器

	Timer* OverallClock = nullptr;//全局计时器

	float deltaTime = 0;//帧间隔时间

	float pauseDelay = 0.f;//世界逻辑暂停时长
	float lastPauseTime = 0.f;//用于记录上次暂停时间

	/** 场景对象、UI容器 **/

	std::unordered_set<Actor*>GameActors;
	std::list<Actor*>GameActors_to_add;
	std::unordered_set<Actor*>GameActors_to_delete;
	std::unordered_set<UserInterface*>GameUIs;
	std::deque<UserInterface*>GameUIs_to_add;
	std::unordered_set<UserInterface*>GameUIs_to_delete;

	/** 场景对象、UI、计时器容器（全局） **/

	std::unordered_set<Actor*>OverallGameActors;
	std::list<Actor*>OverallGameActors_to_add;
	std::unordered_set<UserInterface*>OverallGameUIs;
	std::deque<UserInterface*>OverallGameUIs_to_add;

	std::unordered_set<LayerInterface*>OverallRenders;
	std::unordered_set<Collider*>OverallColliders;
	std::unordered_set<RigidBody*>OverallRigids;
	std::unordered_set<Timer*>GameTimers;

	/**  渲染、碰撞计算、UI鼠标检测容器 **/

	std::set<LayerInterface*, ArtyEngine::LayerSort>GameRenderers;
	std::unordered_set<Collider*>GameColliders;
	std::unordered_set<Collider*>GameColliders_to_clear;
	std::unordered_set<RigidBody*>GameRigids;
	std::set<Collider*, ArtyEngine::ColliderSort>ColliderZones[10][6];
	std::set<LayerInterface*, ArtyEngine::LayerSort>UIDetectZones[6][4];

	/** 游戏单例对象 **/

	GameInstance* gameInstance = nullptr;
	Level* currentLevel = nullptr;
	Camera* mainCamera = nullptr;


	/** 核心逻辑遍历 **/

	//逻辑处理
	void Update(float deltaTime);

	//高精度计算
	void Update_();

	//交互（轴输入）
	void Input();

	//渲染
	void Render();

	//碰撞计算
	void ProcessColliders();

	//处理碰撞事件
	void ProcessCollisions(float deltaTime);

	//Debug模式 
	void Debug();

	//清空数据 
	void WipeData();
};

//游戏世界唯一实例，任何情况都应避免直接使用
extern World mainWorld;