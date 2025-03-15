/**
 * @file	Particle.h
 * @brief	2D粒子系统的实现
 * @author	Arty
 **/


#pragma once
#include"SceneComponent.h"
#include"Tools/VisualInterface.h"



/* 粒子放射形状 */
enum class EParticlePattern :uint8
{
	Center, //中心放射状
	Line //线性放射状
};

/* 粒子消失类型 */
enum class EParticleFadingType :uint8
{
	FadeInAndOut, //渐变
	ExpandAndShrink //放缩
};

/* 单个粒子信息 */
namespace ArtyEngine
{
	struct FParticleInfo
	{
		FVector2D position;//粒子坐标
		int32 index = 0; //当前帧索引
		FVector2D velocity; //速度
		time_point<steady_clock>lastTime;//粒子创建时间
		float alpha = 255.f; //透明度
		float size = 1.f;//大小
		float maxSize = 1.f;//极值大小
		float degree = 0.f;//角度
		FParticleInfo() { lastTime = steady_clock::now(); }
	};
}



/*----------------------------------
			  粒子组件
  ----------------------------------*/
class ParticleSystem final : public SceneComponent, public LayerInterface, public ImageInterface
{
	std::deque<ArtyEngine::FParticleInfo>particles;
	int32 capacity = 0; //粒子容量
	int32 parNum = 0;//粒子生产数目

	float maxSpeed = 1.f; //粒子最大初速度
	float minSpeed = 1.f; //粒子最小初速度
	float graivity = 9.8f; //重力加速度

	float lifeCycle = 1.f; //粒子生命周期
	bool isLoop = true; //是否循环生产
	time_point<steady_clock>lastTime;//用于记录生产
	float interval = 0.1f; //生产间隔
	float fadingInTime = 0; //粒子逐渐出现的时间
	float fadingOutTime = 0; //粒子逐渐消失的时间
	EParticleFadingType fadingType = EParticleFadingType::FadeInAndOut;//粒子逐渐出现或消失的类型

	IMAGE** images = nullptr;//粒子帧数组
	int32 number = 0; //粒子帧数
	FVector2D sizeRange; //大小范围（倍率系数）

	EParticlePattern pattern = EParticlePattern::Center; //粒子放射状模式
	FVector2D unitVector = FVector2D(0, 1); //粒子方向单位向量

	/* 中心放射状属性成员 */
	FVector2D radius{};//内外径
	FVector2D scoop = FVector2D(0, 360);//放射范围

	/* 线性放射状属性成员 */
	float length = 1.f; //线性放射长度
	float angle = 0.f; //线性放射角度
	float offset = 0.f; //线性放射偏移
	bool twoWay = false; //线性放射方向

	void Produce(); //产生粒子

	void DealImage()override {}

public:
	ParticleSystem() :sizeRange(FVector2D(0.75, 1.25)) { lastTime = steady_clock::now(); }

	//加载粒子资源
	void Load(std::string name);

	virtual void Update(float deltaTime)override;

	virtual void Render()override;

	//设置粒子容量
	void SetCapacity(int32 capacity) { this->capacity = capacity; }

	//设置粒子发射最大初速度
	void SetMaxSpeed(float speed) { this->maxSpeed = speed; }

	//设置粒子发射最小初速度
	void SetMinSpeed(float speed) { this->minSpeed = speed; }

	//设置粒子重力
	void SetGravity(float graivity) { this->graivity = graivity; }

	//设置单个粒子生命周期 
	void SetLifeCycle(float lifeCycle) { this->lifeCycle = lifeCycle; }

	//设置粒子是否持续生产 
	void SetIsLoop(bool isLoop) { this->isLoop = isLoop; }

	//设置粒子生产间隔
	void SetInterval(float interval) { this->interval = interval; }

	//设置粒子逐渐出现时间 
	void SetFadingInTime(float fadingTime) { this->fadingInTime = fadingTime; }

	//设置粒子逐渐消失时间 
	void SetFadingOutTime(float fadingTime) { this->fadingOutTime = fadingTime; }

	//设置粒子渐变消失类型
	void SetFadingType(EParticleFadingType fadingType) { this->fadingType = fadingType; }

	//设置粒子模式
	void SetPattern(EParticlePattern pattern) { this->pattern = pattern; }

	/**
	 * @brief 设置大小范围（倍率系数）
	 * @param[in] lower			    最小倍率
	 * @param[in] upper				最大倍率
	 **/
	void SetSizeRange(float lower, float upper) { sizeRange = {lower,upper}; }

	/**
	 * @brief 设置中心放射状参数
	 * @param[in] radius			放射圆环的内外径
	 * @param[in] scoop				放射扇形的角度范围
	 **/
	void SetCenter(FVector2D radius, FVector2D scoop = FVector2D(0, 360));

	/**
	 * @brief 设置线性放射状参数
	 * @param[in] length			放射线段长度
	 * @param[in] angle	            放射方向角度(线段法向)
	 * @param[in] offset	        放射偏移
	 * @param[in] twoWay	        是否为双向
	 **/
	void SetLine(float length, float angle, float offset = 0.f, bool twoWay = false);

	virtual void RegisterDontDestroy()override;
};