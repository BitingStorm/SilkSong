/**
 * @file	UserInterface.h
 * @brief   UI��ģ�壬�������ɲ�����uiС����
 * @author	Arty
 **/

#pragma once
#include"Widgets.h"



/*----------------------------------
			    UI�� 
  ----------------------------------*/
class UserInterface : public Object, public ITimerHandler
{
	friend Panel;
	friend class WidgetComponent;

	std::unordered_set<Widget*>widgets;

	void BeginPlay()override {}
	void EndPlay()override {}

protected:
	Widget* const rootCanvas = new Widget;

public:
	UserInterface();
	~UserInterface() { for (auto& widget : widgets)delete widget; }

	virtual void Update(float deltaTime);

	void ShowInfoBoxs();

	void DrawDebugRect();

	//���widgetС������UIģ�����
	template<class T>
	T* AddWidget()
	{
		T* gameWid = new T;
		if (Widget* buf = static_cast<Widget*>(gameWid))
		{
			widgets.insert(buf);
			return gameWid;
		}
		delete gameWid;
		return nullptr;
	}

	//��UI��ӵ��ӿ�
	void AddToViewport();

	//��UI���ӿ���ʱ����
	void HideFromViewport();

	//��UI���ӿ��Ƴ�
	void RemoveFromViewport();
};