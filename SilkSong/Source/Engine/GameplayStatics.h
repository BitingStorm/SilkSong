/**
 * @file	GameplayStatics.h
 * @brief   ȫ�־�̬�ⶨ�壬�ṩ������Ϸ�����߼�����Ľӿڣ���������ʲ�������Ϸ����
 * @author	Arty
 **/



#pragma once
#include"CoreMinimal.h"
#include"Core/World.h"


/*----------------------------------
			��Ϸ�߼���̬��
  ----------------------------------*/
class GameplayStatics final
{
public:
	//��ȡ��Ϸʵ��
	static GameInstance* GetGameInstance();
	
	//��ȡ��ҿ�����
	static class Controller* GetController();

	//������Ϸ����
	template<typename T>
	static T* CreateObject(FVector2D pos = FVector2D(0, 0), float angle = 0, FVector2D scale = FVector2D(1, 1));

	//ͨ���������Ϸ����
	template<typename T>
	static std::vector<T*>FindObjectsOfClass();

	//ͨ���������Ϸ����
	template<typename T>
	static T* FindObjectOfClass();

	//ͨ����ǩ��������Ϸ����
	template<typename T>
	static T* FindObjectOfName(std::string tagName);

	//����UI����
	template<class T>
	static T* CreateUI();

	//���ص�ͼ
	static void OpenLevel(std::string levelName);

	//�˳���Ϸ
	static void QuitGame();

	/**
	 * @brief �����
	 * @param[in] intensity			  ��ǿ�ȣ����ȣ�
	 * @param[in] decay               ��˥��ϵ������ΧΪ1`100��Խ����˥������Խ��
	 **/
	static void PlayCameraShake(int intensity, int decay = 10);

	//��ȡ��Ϸ��ʼʱ��
	static double GetTimeSeconds();

	//���ø�˹ģ������ݣ�ÿһ֡���ᴦ���ܺ����ܡ��������������ʹ��������������Ǵ�����
	static void SetGaussianFilterOn(bool enable,int level = 2);

	//��ͣ��Ϸ����
	static void Pause(float delay);

	//������ЧƬ��
	static void PlaySound2D(std::string name);

	//ȫ�ֻ����󣬸ö��󳡾��л��󲻻ᱻɾ��
	static void DontDestroyOnLoad(class Actor* obj);

	//ȫ�ֻ����󣬸ö��󳡾��л��󲻻ᱻɾ��
	static void DontDestroyOnLoad(class UserInterface* obj);

	//����Ļ����Ͷ�䵽����
	static FVector2D ProjectScreenToWorld(FVector2D pos);

	//����������Ͷ�䵽��Ļ
	static FVector2D ProjectWorldToScreen(FVector2D pos);
};






template<typename T>
inline T* GameplayStatics::CreateObject(FVector2D pos, float angle, FVector2D scale)
{
	T* pObj = new T;
	if (pObj && static_cast<Actor*>(pObj))
	{
		mainWorld.GameActors_to_add.push_back(pObj);
		pObj->SetLocalPosition(pos);
		pObj->SetLocalRotation(angle);
		pObj->SetLocalScale(scale);
		return pObj;
	}
	delete pObj;
	return nullptr;
}

template<typename T>
inline std::vector<T*> GameplayStatics::FindObjectsOfClass()
{
	std::vector<T*> result;
	result.reserve(mainWorld.GameActors.size());
	for (auto& obj : mainWorld.GameActors) 
	{
		if (T* pObj = dynamic_cast<T*>(obj))result.push_back(pObj);
	}
	return result;
}

template<typename T>
inline T* GameplayStatics::FindObjectOfClass()
{
	for (auto& obj : mainWorld.GameActors)
	{
		if (T* pObj = dynamic_cast<T*>(obj))return pObj;
	}
	return nullptr;
}

template<typename T>
inline T* GameplayStatics::FindObjectOfName(std::string tagName)
{
	for (auto& obj : mainWorld.GameActors)
	{
		if (obj->GetName() == tagName)return obj;
	}
	return nullptr;
}

template<class T>
inline T* GameplayStatics::CreateUI()
{
	T* pUI = new T;
	if (pUI && static_cast<UserInterface*>(pUI)) 
	{
		mainWorld.GameUIs_to_add.push_back(pUI);
		return pUI;
	}
	delete pUI;
	return nullptr;
}

