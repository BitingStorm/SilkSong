/**
 * @file	AudioPlayer.h
 * @brief	媒体播放器定义
 * @author	Arty
 **/

#pragma once
#include "SceneComponent.h"



 /*----------------------------------
			   媒体播放器
   ----------------------------------*/
class AudioPlayer final :public SceneComponent
{
	DEFINE_SUPER(SceneComponent)

	std::unordered_set<std::string>paths;

	float innerRadius = 0;//音源内径
	float outerRadius = 0;//音源外径
	bool bSpacial = false;//支持空间感的播放器时间开销较大，慎用
	int32 volume = 1000;//媒体音量大小，范围为0~1000

	int32 currentVol = 0;
	
public:
	virtual ~AudioPlayer();

	virtual void Update(float deltaTime)override;

	//是否支持空间感
	void SetSpacial(bool spacial) { bSpacial = spacial; }

	//设置内外径，从内径到外径声源递减至零
	void SetRadius(float inner, float outer) 
	{
		innerRadius = (inner < 0 ? 0 : inner);
		outerRadius = (outer < 0 ? 0 : outer);
	}

	//获取音量大小
	int32 GetVolume()const { return volume; }

	//设置音量大小
	void SetVolume(int32 vol);

	//播放音乐
	void Play(std::string name, bool repeat = false);

	//回收音乐资源（节省内存，但再次播放会消耗较大性能）
	void Stop(std::string name);

	//停止音乐
	void Close(std::string name);

	//暂停音乐
	void Pause(std::string name);

	//继续音乐
	void Resume(std::string name);

	//检测音乐是否在播放
	bool IsPlaying(std::string name);

private:
	int32 Reflect(int32 x);
};