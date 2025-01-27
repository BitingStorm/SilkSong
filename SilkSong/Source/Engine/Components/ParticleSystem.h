/**
 * @file	Particle.h
 * @brief	2D����ϵͳ��ʵ��
 * @author	Arty
 **/


#pragma once
#include"SceneComponent.h"
#include"Tools/VisualInterface.h"
#include<chrono>
#include<queue>


using std::chrono::duration;
using std::chrono::steady_clock;
using std::chrono::time_point;


/* ���ӷ�����״ */
enum class ParticlePattern :uint8
{
	Center, //���ķ���״
	Line //���Է���״
};

/* ������ʧ���� */
enum class ParticleFadingType :uint8
{
	FadeInAndOut, //����
	ExpandAndShrink //����
};

/* ����������Ϣ */
struct ParticleInfo
{
	Vector2D position;//��������
	int index = 0; //��ǰ֡����
	Vector2D velocity; //�ٶ�
	time_point<steady_clock>lastTime;//���Ӵ���ʱ��
	float alpha = 255.f; //͸����
	float size = 1.f;//��С
	float maxSize = 1.f;//��ֵ��С 
	ParticleInfo() { lastTime = steady_clock::now(); }
};



/*----------------------------------
			  �������
  ----------------------------------*/
class ParticleSystem final : public SceneComponent, public LayerInterface, public ImageInterface
{
	std::deque<ParticleInfo>particles;
	int capacity = 0; //��������

	float maxSpeed = 1.f; //���������ٶ�
	float minSpeed = 1.f; //������С���ٶ�
	float graivity = 9.8f; //�������ٶ�

	float lifeCycle = 1.f; //������������
	bool isLoop = true; //�Ƿ�ѭ������
	time_point<steady_clock>lastTime;//���ڼ�¼����
	float interval = 0.1f; //�������
	float fadingInTime = 0; //�����𽥳��ֵ�ʱ��
	float fadingOutTime = 0; //��������ʧ��ʱ��
	ParticleFadingType fadingType = ParticleFadingType::FadeInAndOut;//�����𽥳��ֻ���ʧ������

	IMAGE** images = nullptr;//����֡����
	int number = 0; //����֡��
	Vector2D sizeRange; //��С��Χ������ϵ����

	ParticlePattern pattern = ParticlePattern::Center; //���ӷ���״ģʽ
	Vector2D unitVector = Vector2D(0, 1); //���ӷ���λ����

	/* ���ķ���״���Գ�Ա */
	Vector2D radius = Vector2D(0, 0);//���⾶
	Vector2D scoop = Vector2D(0, 360);//���䷶Χ

	/* ���Է���״���Գ�Ա */
	float length = 1.f; //���Է��䳤��
	float angle = 0.f; //���Է���Ƕ�

	void Produce(); //��������

public:
	ParticleSystem():sizeRange(Vector2D(0.75,1.25)) {lastTime = steady_clock::now();}

	//����������Դ
	void Load(std::string name);

	virtual void Update(float deltaTime)override;

	virtual void Render()override;

	//������������
	void SetCapacity(int capacity) { this->capacity = capacity; }

	//�������ӷ��������ٶ�
	void SetMaxSpeed(float speed) { this->maxSpeed = speed; }

	//�������ӷ�����С���ٶ�
	void SetMinSpeed(float speed) { this->minSpeed = speed; }

	//������������
	void SetGravity(float graivity) { this->graivity = graivity; }

	//���õ��������������� 
	void SetLifeCycle(float lifeCycle) { this->lifeCycle = lifeCycle; }

	//���������Ƿ�������� 
	void SetIsLoop(bool isLoop) { this->isLoop = isLoop; }

	//���������������
	void SetInterval(float interval) { this->interval = interval; }

	//���������𽥳���ʱ�� 
	void SetFadingInTime(float fadingTime) { this->fadingInTime = fadingTime; }

	//������������ʧʱ�� 
	void SetFadingOutTime(float fadingTime) { this->fadingOutTime = fadingTime; }

	//�������ӽ�����ʧ����
	void SetFadingType(ParticleFadingType  fadingType) { this->fadingType = fadingType; }

	//��������ģʽ
	void SetPattern(ParticlePattern pattern) { this->pattern = pattern; }

	/**
	 * @brief ���ô�С��Χ������ϵ����
	 * @param[in] lower			    ��С����
	 * @param[in] upper				�����
	 **/
	void SetSizeRange(float lower, float upper) { sizeRange = {lower,upper}; }

	/**
	 * @brief �������ķ���״����
	 * @param[in] radius			����Բ�������⾶
	 * @param[in] scoop				�������εĽǶȷ�Χ
	 **/
	void SetCenter(Vector2D radius, Vector2D scoop = Vector2D(0, 360)) { this->radius = radius; this->scoop = scoop; }

	/**
	 * @brief �������Է���״����
	 * @param[in] length			�����߶γ���
	 * @param[in] angle	            �߶���ˮƽ�н�
	 **/
	void SetLine(float length, float angle) { this->length = length; this->angle = angle; }
};