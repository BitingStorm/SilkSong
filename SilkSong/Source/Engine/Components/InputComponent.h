/**
 * @file	InputComponent.h
 * @brief	��������������߼�����
 * @author	Arty
 **/


#pragma once
#include "ActorComponent.h"
#include <easyx.h>


/* �����ź����� */
enum class EInputType :uint8
{
	Pressed,
	Released,
	Holding,
	DoubleClick
};

/* ���� */
enum class EKeyCode :int32
{
	VK_LButton = 0x01,
	VK_RButton,
	VK_MButton = 0x04,
	VK_Backspace = 0x08,
	VK_Tab,
	VK_Enter = 0x0D,
	VK_Shift = 0x10,
	VK_Ctrl, 
	VK_Alt,
	VK_Esc = 0x1B,
	VK_Space = 0x20,
	VK_Delete = 0x2E,
	VK_1 = 0x30, VK_2, VK_3, VK_4, VK_5,
	VK_6, VK_7, VK_8, VK_9, VK_0,
	VK_A = 0x41,
	VK_B, VK_C, VK_D,
	VK_E, VK_F, VK_G,
	VK_H, VK_I, VK_J,
	VK_K, VK_L, VK_M,
	VK_N, VK_O, VK_P,
	VK_Q, VK_R, VK_S,
	VK_T, VK_U, VK_V,
	VK_W, VK_X, VK_Y,
	VK_Z,
	VK_Num0 = 0x60, VK_Num1, VK_Num2, VK_Num3, VK_Num4,
	VK_Num5, VK_Num6, VK_Num7, VK_Num8, VK_Num9,
	VK_Multiply = 0x6A,
	VK_Add,
	VK_Subtract = 0x6D,
	VK_Decimal,
	VK_Devide,
	VK_Fn1 = 0x70, VK_Fn2, VK_Fn3, VK_Fn4, VK_Fn5, VK_Fn6,
	VK_Fn7, VK_Fn8, VK_Fn9, VK_Fn10, VK_Fn11, VK_Fn12,
	VK_Colon = 0xBA,
	VK_Plus,
	VK_Comma,
	VK_Minus,
	VK_Period,
	VK_Question,
	VK_Tilde,
	VK_LBracket = 0xDB,
	VK_Backslash,
	VK_RBracket,
	VK_Quotation,
};

/* ����ӳ����Ϣ */
struct FKeyBindInfo
{
	std::function<void()> func;
	EInputType type = EInputType::Pressed;
	int32 pressFlag = 0;
	float lastTime = 0;
};


/*----------------------------------
			  ���뽻�����
  ----------------------------------*/
class InputComponent final: public ActorComponent
{
public:
	/**
	 * @brief ���ð���ӳ��
	 * @param[in] mappingName			  �¼���
	 * @param[in] value                   ����
	 **/
	void SetMapping(std::string mappingName, EKeyCode value);

	/**
	 * @brief �󶨰����¼�ӳ��
	 * @param[in] actionName		      �¼���
	 * @param[in] type                    ������������
	 * @param[in] obj	                  �ຯ����������ָ��
	 * @param[in] function                �ຯ��ָ��
	 **/
	template<typename T>
	void BindAction(std::string actionName, EInputType type, T* obj, void(T::* func)())
	{
		if (mappings.find(actionName) != mappings.end())
		{
			actionCallbacks.insert({ actionName, { std::bind(func, obj),type,false } });
		}
	}

	/**
     * @brief �󶨰����¼�ӳ��
     * @param[in] actionName		      �¼���
     * @param[in] type                    ������������
     * @param[in] function                ������װ
     **/
	void BindAction(std::string actionName, EInputType type, std::function<void()>func);

	//��ȡ��ǰ������
	BYTE GetCurrentKeyCode();

	//�Ƿ����κΰ���������
	static bool IsAnyKeyPressed();

	//ĳ�����Ƿ񱻰���
	static bool IsKeyPressed(EKeyCode keycode);

	//��ȡ�����Ļ����
	static FVector2D GetMousePosition();
	
	//�������Ƿ���
	static bool IsMouseButtonPressed();

	//�����������
	static void EnableInput(bool enable);

	//Action(��Ϊ�¼���˲ʱ����������Ƶ����֡��Ӱ��)
	void PeekInfo();
	
	//Axis(���¼�����������������Ƶ�ʺ㶨)
	void PeekInfo_();

	void MouseTick();

private:
	std::unordered_map<std::string, EKeyCode>mappings;
	std::unordered_map<std::string, FKeyBindInfo>actionCallbacks;
	std::unordered_map<std::string, FKeyBindInfo>axisCallbacks;

	static FVector2D mousePos;//����ڻ�ͼ��������
	static bool bActive;//�Ƿ����ý���
	ExMessage msg;
};
