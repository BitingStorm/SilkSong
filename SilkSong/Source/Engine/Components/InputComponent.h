/**
 * @file	InputComponent.h
 * @brief	��������������߼�����
 * @author	Arty
 **/


#pragma once
#include "ActorComponent.h"
#include <unordered_map>
#include <functional>
#include <easyx.h>


/* �����ź����� */
enum class InputType :uint8
{
	Pressed,
	Released,
	Holding,
	DoubleClick
};

/* ���� */
enum KeyCode :int32
{
	VK_LButton = 1,
	VK_RButton,
	VK_MButton = 4,
	VK_Tab = 9,
	VK_Enter = 13,
	VK_Shift = 16,
	VK_Ctrl,
	VK_Esc = 27,
	VK_Space = 32,
	VK_A = 0x41,
	VK_B,VK_C,VK_D,
	VK_E,VK_F,VK_G,
	VK_H,VK_I,VK_J,
	VK_K,VK_L,VK_M,
	VK_N,VK_O,VK_P,
	VK_Q,VK_R,VK_S,
	VK_T,VK_U,VK_V,
	VK_W,VK_X,VK_Y,
	VK_Z,
};

/* ����ӳ����Ϣ */
struct KeyBindInfo
{
	std::function<void()> func;
	InputType type = InputType::Pressed;
	int pressFlag = 0;
	float lastTime = 0;
};


/*----------------------------------
			  ���뽻�����
  ----------------------------------*/
class InputComponent final: public ActorComponent
{
public:
	//���ð���ӳ��
	void SetMapping(std::string mappingName, KeyCode value);

	//�󶨰����¼�ӳ�䣨˲ʱ����������Ƶ����֡��Ӱ�죩
	template<typename T>
	void BindAction(std::string actionName,InputType type, T* obj, void(T::*func)())
	{
	    if(mappings.find(actionName)!= mappings.end())
		actionCallbacks.insert({ actionName, { std::bind(func, obj),type,false } });
	}

	//�󶨰����¼�ӳ�䣨˲ʱ����������Ƶ����֡��Ӱ�죩
	void BindAction(std::string actionName, InputType type, std::function<void()>func);


	//��ȡ�����Ļ����
	static Vector2D GetMousePosition();
	
	//�������Ƿ���
	static bool IsMouseButtonPressed();

	//�����������
	static void EnableInput(bool enable);

	//action
	void PeekInfo();
	
	//axis
	void PeekInfo_();

	void MouseTick();

private:
	std::unordered_map<std::string, KeyCode>mappings;
	std::unordered_map<std::string, KeyBindInfo>actionCallbacks;
	std::unordered_map<std::string, KeyBindInfo>axisCallbacks;

	static Vector2D mousePos;//����ڻ�ͼ��������
	static bool bActive;//�Ƿ����ý���
	ExMessage msg;
};
