#include"World.h"
#include"GameplayStatics.h"
#include"Tools/LevelManager.h"
#include"Tools/Timer.h"
#include"Tools/VisualInterface.h"
#include"Components/Camera.h"
#include"Components/Collider.h"
#include"Components/RigidBody.h"
#include"Objects/Controller.h"
#include"UI/UserInterface.h"
#include"Objects/Level.h"



int Object::number = 0;
extern World mainWorld{};


bool ArtyEngine::LayerSort::operator()(const LayerInterface* a, const LayerInterface* b) const
{
	if (a->GetLayer() == b->GetLayer()) return a < b;
	else return a->GetLayer() < b->GetLayer();
}

bool ArtyEngine::ColliderSort::operator()(const Collider* a, const Collider* b)const
{
	if (a->GetLayer() == b->GetLayer()) return a < b;
	else return a->GetLayer() < b->GetLayer();
}




void World::Update(float deltaTime)
{
	if (pauseDelay > 0 && OverallClock->GetDelay() > lastPauseTime + pauseDelay)
	{
		pauseDelay = 0.f;
	}

	if (pauseDelay == 0)
	{
		for (int i = 0; i < 4; i++)
		{
			ProcessCollisions(deltaTime * 0.25f);
		}
	}
	ProcessColliders();

	GameplayStatics::GetController()->PeekInfo();//交互处理（点输入）

	if (pauseDelay == 0 && !levelManager->level_to_delete)
	{
		currentLevel->Update(deltaTime);
		for (auto& obj : GameActors)obj->Update(deltaTime);
		for (auto& obj : OverallGameActors)obj->Update(deltaTime);
	}

	{
		std::lock_guard<std::mutex> lock(updateMutex);
		
		for (auto& obj : GameActors_to_add)
		{
			GameActors.insert(obj);
			obj->BeginPlay();
		}
		GameActors_to_add.clear();

		for (auto& obj : OverallGameActors_to_add)
		{
			GameActors.erase(obj);
			OverallGameActors.insert(obj);
		}
		OverallGameActors_to_add.clear();

		for (auto& obj : GameActors_to_delete)
		{
			obj->EndPlay();
			GameActors.erase(obj);
			OverallGameActors.erase(obj);//全局对象数量很少，不必额外判断obj是否是全局对象以节省时间
			delete obj;
		}
		GameActors_to_delete.clear();
	}

	for (auto& obj : GameUIs)obj->Update(deltaTime);
	for (auto& obj : OverallGameUIs)obj->Update(deltaTime);

	{
		std::lock_guard<std::mutex> lock(updateMutex);

		for (auto& obj : GameUIs_to_add)
		{
			GameUIs.insert(obj);
		}
		GameUIs_to_add.clear();

		for (auto& obj : OverallGameUIs_to_add)
		{
			GameUIs.erase(obj);
			OverallGameUIs.insert(obj);
		}
		OverallGameUIs_to_add.clear();

		for (auto& obj : GameUIs_to_delete)
		{
			GameUIs.erase(obj);
			OverallGameUIs.erase(obj);//全局对象数量很少，不必额外判断obj是否是全局对象以节省时间
			delete obj;
		}
		GameUIs_to_delete.clear();
	}

	if (pauseDelay == 0 && !levelManager->level_to_delete)
	{
		for (auto& obj : GameTimers)
		{
			obj->Execute();
		}
	}

	{
		std::lock_guard<std::mutex> lock(updateMutex);
		levelManager->RefreshLevel();//检查关卡更新
	}
}


void World::ProcessColliders()
{
	/**
	 * 碰撞删除信息更新
	 **/
	for (auto& it : GameColliders)it->Erase();

	/**
	 * 碰撞体移除更新
	 **/
	{
		std::lock_guard<std::mutex> lock(updateMutex);

		for (auto& it : GameColliders_to_clear)it->Clear();
		GameColliders_to_clear.clear();
	}
}

void World::ProcessCollisions(float deltaTime)
{
	for (auto& it : GameRigids)it->PreciseUpdate(deltaTime);

	for (auto& it : GameColliders)it->ColliderZoneTick();

	/**
     * 碰撞插入信息更新
     **/
	for (auto& arr_i : ColliderZones)for (auto& arr_j : arr_i) if (!arr_j.empty())
		for (auto& me : arr_j)for (auto& he : arr_j) if (he != me) { me->Insert(he); if (levelManager->level_to_delete)return; }
}


void World::Render()
{
	if (videoLock)
	{
		return;
	}

	cleardevice();

	for (auto& obj : GameRenderers) obj->Render();
	for (auto& obj : GameUIs)obj->ShowInfoBoxs();

	ImageToolkit::ApplyGaussianFilterOnScreen();

	Debug();

	FlushBatchDraw(); 
}

void World::Update_()
{
	mainCamera->Calculate();
	GameplayStatics::GetController()->MouseTick();
}

void World::Input()
{
	GameplayStatics::GetController()->PeekInfo_();
}

void World::Debug()
{
#ifdef _DEBUG
	for (auto& obj : GameColliders)
		obj->DrawDebugLine();
	for (auto& obj : GameActors)
		obj->DrawDebugPosition();
	for (auto& obj : GameUIs)
		obj->DrawDebugRect();
	for (auto& obj : OverallGameActors)
		obj->DrawDebugPosition();
	for (auto& obj : OverallGameUIs)
		obj->DrawDebugRect();

	/*setlinecolor(RGB(255, 165, 0));
	setlinestyle(PS_SOLID | PS_JOIN_BEVEL);
	for (int i = 1; i < 6; i++)
	{
		FVector2D pos1 = (FVector2D(-2000, 200 * i) - mainCamera->GetVirtualPosition())
			* 20.f / mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
		FVector2D pos2 = (FVector2D(2000, 200 * i) - mainCamera->GetVirtualPosition())
			* 20.f / mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
		line(pos1.x, pos1.y, pos2.x, pos2.y);
	}
	for (int j = 1; j < 10; j++)
	{
		FVector2D pos1 = (FVector2D(j * 400 - 2000, 0) - mainCamera->GetVirtualPosition())
			* 20.f / mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
		FVector2D pos2 = (FVector2D(j * 400 - 2000, 1200) - mainCamera->GetVirtualPosition())
			* 20.f / mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
		line(pos1.x, pos1.y, pos2.x, pos2.y);
	}
	settextstyle(25, 10, "Arial");
	settextcolor(WHITE);
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			FVector2D pos = (FVector2D(j * 400 - 2000, i * 200) - mainCamera->GetVirtualPosition())
				* 20.f / mainCamera->GetVirtualSpringArmLength() + FVector2D(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f);
			outtextxy(pos.x, pos.y, std::to_string(ColliderZones[j][i].size()).c_str());
		}
	}*/
#endif

	static int FPS = 0;
	static int number = 0;
	number++;

	settextstyle(25, 10, "Arial");
	settextcolor(RGB(255, 200, 15));
	outtextxy(0, 0, (std::string("FPS: ") + std::to_string(FPS)).c_str());
	if (FPSClock->GetDelay() >= 1.0)
	{
		FPS = number;
		FPSClock->Reset();
		number = 0;
	}
}

void World::WipeData()
{
	for (auto& obj : GameActors)delete obj;
	for (auto& obj : GameUIs)delete obj;
	for (auto& obj : GameActors_to_add)delete obj;
	for (auto& obj : GameUIs_to_add)delete obj;

	GameActors.clear();
	GameActors_to_add.clear();
	GameActors_to_delete.clear();
	GameUIs.clear();
	GameUIs_to_add.clear();
	GameUIs_to_delete.clear();
	GameColliders.clear();
	GameColliders_to_clear.clear();
	GameRigids.clear();
	GameRenderers.clear();
	for (auto& obj : OverallRenders)
	{
		GameRenderers.insert(obj);
	}
	for (auto& obj : OverallColliders)
	{
		GameColliders.insert(obj);
		obj->Init();
	}
	for (auto& obj : OverallRigids)
	{
		GameRigids.insert(obj);
	}
	for (auto& arr_i : ColliderZones)for (auto& arr_j : arr_i)arr_j.clear();
	for (auto& arr_i : UIDetectZones)for (auto& arr_j : arr_i)arr_j.clear();
}