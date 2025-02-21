#include"World.h"
#include"GameplayStatics.h"
#include"Tools/LevelManager.h"
#include"Tools/Timer.h"
#include"Tools/VisualInterface.h"
#include"Components/Camera.h"
#include"Components/Collider.h"
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
	ProcessColliders();
	
	//交互处理（点输入）
	GameplayStatics::GetController()->PeekInfo();

	currentLevel->Update(deltaTime);

	for (auto& obj : GameActors)obj->Update(deltaTime);
	for (auto& obj : OverallGameActors)obj->Update(deltaTime);

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
			auto it = std::find(GameActors_to_add.begin(), GameActors_to_add.end(), obj);
			if (it != GameActors_to_add.end())
			{
				GameActors_to_add.erase(it);
			}
			mainWorld.GameActors.erase(obj);
			mainWorld.OverallGameActors.insert(obj);
		}
		OverallGameActors_to_add.clear();

		for (auto& obj : GameActors_to_delete)
		{
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
			auto it = std::find(GameUIs_to_add.begin(), GameUIs_to_add.end(), obj);
			if (it != GameUIs_to_add.end())
			{
				GameUIs_to_add.erase(it);
			}
			mainWorld.GameUIs.erase(obj);
			mainWorld.OverallGameUIs.insert(obj);
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

	for (auto& obj : GameTimers)
	{
		obj->Execute();
	}

	{
		std::lock_guard<std::mutex> lock(updateMutex);
		levelManager->RefreshLevel();//检查关卡更新
	}
}


void World::ProcessColliders()
{
	/**
	 * 碰撞插入信息更新
	 **/
	for (auto& arr_i : ColliderZones)for (auto& arr_j : arr_i) if (!arr_j.empty())
		for (auto& me : arr_j)for (auto& he : arr_j) if (he != me) me->Insert(he);

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


void World::Render()
{
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
	GameRenderers.clear();
	for (auto& arr_i : ColliderZones)for (auto& arr_j : arr_i)arr_j.clear();
}