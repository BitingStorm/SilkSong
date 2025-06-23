/**
 * @file	VideoPlayer.h
 * @brief	视频播放器定义
 * @author	Arty
 **/

#pragma once
#include "SceneComponent.h"
#include <dshow.h>

DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(VideoEvent)

 /*----------------------------------
			   视频播放器
   ----------------------------------*/
class VideoPlayer final :public SceneComponent
{
public:
	VideoPlayer();
	~VideoPlayer();

	virtual void Update(float deltaTime)override;

	//加载视频资源
	void Load(std::string name);
	
	//开始播放
	void Play();
	
	//暂停播放
	void Pause();
	
	//中止播放
	void Stop();

	//设置屏幕宽高
	void SetRect(FRect rect);

	VideoEvent OnVideoEnd;
private:
	IGraphBuilder* m_pGraph;
	IMediaControl* m_pControl;
	IVideoWindow* m_pVW;
	IMediaEventEx* m_pEvent;
	FRect renderBox;
};