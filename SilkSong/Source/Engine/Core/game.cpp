#include"CoreMinimal.h"
#include"World.h"

#include"Tools/ResourceManager.h"
#include"Tools/CollisionManager.h"
#include"Tools/LevelManager.h"

#include"GameplayStatics.h"

#include <easyx.h>
#include <windows.h>
#pragma comment(lib,"winmm.lib")

#include <thread>
#include <immintrin.h>

/*----------------------------------
			   引擎
  ----------------------------------*/
class Engine
{
public:
	static void Init()
	{
		initgraph(WIN_WIDTH, WIN_HEIGHT);

		//系统Debug绘制参数
		setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
		setfillcolor(LIGHTBLUE);
		setlinecolor(LIGHTBLUE);
		
		//系统文字参数
		setbkmode(TRANSPARENT);
		settextstyle(25, 10, "Arial");
		settextcolor(WHITE);
	

		BeginBatchDraw();


		//初始化资源管理器
		mainWorld.resourcePool = new ResourceManager;
		mainWorld.resourcePool->Initialize();
		

		//初始化碰撞管理器
		mainWorld.collisionManager = new CollisionManager;
		mainWorld.collisionManager->Initialize();
		

		//初始化游戏世界
		mainWorld.levelManager = new LevelManager;
		mainWorld.levelManager->Initialize();
		mainWorld.currentLevel->BeginPlay();
		mainWorld.FPSClock = new Timer;
		mainWorld.OverallClock = new Timer;
	}

	static void Tick(float deltaTime)//帧更新
	{
		mainWorld.deltaTime = 0;
		mainWorld.Update(deltaTime);
		mainWorld.Render();
	}

	static void Tick_()//高频更新
	{
		{
			std::lock_guard<std::mutex> lock(mainWorld.updateMutex);
			mainWorld.Input(); 
		}
		mainWorld.Update_();
	}

	static void Run()
	{
		double lastTime = 0;
		while (true)
		{
			lastTime = GameplayStatics::GetTimeSeconds();
			Engine::Tick(mainWorld.deltaTime);
			if (bFrameLimitOn)
			{
				double interval = double(1) / double(MaxFrame) - (GameplayStatics::GetTimeSeconds() - lastTime) - 0.001;
				if (interval > 0)std::this_thread::sleep_for(duration<double>(interval));
			}
			else _mm_pause();
			mainWorld.deltaTime += float(GameplayStatics::GetTimeSeconds() - lastTime);
		}
	}

	static void Exit()
	{
		delete mainWorld.gameInstance; 
	}
};



void CALLBACK timecallback(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR)
{
	Engine::Tick_();
}


int main()
{
	Engine::Init();
	
	timeSetEvent(1, 0, timecallback,0, TIME_CALLBACK_FUNCTION | TIME_PERIODIC);
	
	Engine::Run();

	Sleep(INFINITE);
	return 0;
}


