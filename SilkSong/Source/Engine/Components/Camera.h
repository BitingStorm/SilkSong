/**
 * @file	Camera.h
 * @brief	2D����������
 * @author	Arty
 **/

#pragma once
#include "SceneComponent.h"
#include "Objects/Actor.h"


/*----------------------------------
			  ��������
  ----------------------------------*/
class Camera final:public SceneComponent
{
	friend class SpriteRenderer;
	friend class ParticleSystem;
	friend class CircleCollider;
	friend class BoxCollider;
	friend class World;
	friend void Actor::DrawDebugPosition()const;

public:
	virtual void BeginPlay()override;

	//�����������
	void SetMainCamera();

	//��������ƶ�ƽ����
	void SetSmoothness(int smooth);

	//���þ�����ֵ
	void SetDistanceThreshold(float threshold);

	//����������ɳ���
	void SetSpringArmLength(float length);

	//�����������ƽ����
	void SetSpringArmSmoothness(int smooth);

	//�����
	void ShakeCamera(int intensity,int decay = 10);

	//��������ɻ���
	void SetRectFrame(const Rect& frame);

	//��ȡ�������λ��
	Vector2D GetVirtualPosition()const;

private:
	Transform transform_virtual;//���������������

	Rect frame;//����ɻ��

	float distanceThreshold = 100.f;//������ֵ0`500
	int smoothness = 30;//ƽ����0~100


	float shakeIntensity = 0;//��ǿ��0~100
	bool shakeFlag = false;//�𶯱�־λ
	int shakeDecay = 5;//��˥��ϵ��1`100
	Vector2D lastShakeVector;//��¼�ϴ���ƫ������

	float springArmLength = 20.f;//���ɳ���1~10000
	float springArmLength_virtual = 100.f;//���ⵯ�ɳ���
	int smoothnessForSpringArm = 20;//����ƽ����0~100


	Vector2D Lerp(Vector2D start, Vector2D end, float t);//���Բ�ֵ��Vector2D��
	float Lerp(float start, float end, float t);//���Բ�ֵ��float��

	//ƽ������
	float SmoothStep(float x);

	void Calculate();
};