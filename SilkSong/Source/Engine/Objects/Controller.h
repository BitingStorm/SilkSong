/**
 * @file	Controller.h
 * @brief   ��ҿ������ඨ�壬��ÿ�������б�����ڵĶ��󣨲�Ψһ����ͬһʱ��ֻ�ܴ���һ��������������߼�����������ҽ����߼�
 * @author	Arty
 **/


#pragma once
#include"Actor.h"
#include"Components/InputComponent.h"
#include"Components/Collider.h"


class Camera;


/*----------------------------------
			 ��ҿ�������
  ----------------------------------*/
class Controller :public Actor
{
	DEFINE_SUPER(Actor)

	InputComponent* inputComponent;
	Camera* camera;

protected:
	virtual void SetupInputComponent(InputComponent* inputComponent) {}

public:
	Controller();

	virtual void BeginPlay() override;

	//���µ�������Ϣ
	void PeekInfo();

	//������������Ϣ
	void PeekInfo_();

	//���������Ϣ
	void MouseTick();

	//��ȡ�����������ϵλ��
	FVector2D GetCursorPosition() const;

	//��ȡ����Ƿ���
	bool IsMouseClicked() const;

	/**
	 * @brief ��ȡ�������λ���������Ϣ�����������ӵ����ײ�壩
	 * @return  HitResult ��������ϵ���������λ������������Ϣ
	 **/
	HitResult GetHitResultUnderCursor() const;

	//�����������
	void EnableInput(bool enable);
};