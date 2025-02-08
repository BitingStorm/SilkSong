/**
 * @file	Timer.h
 * @brief   计时器类定义，提供精确的计时器回调处理逻辑
 * @author	Arty
 **/

#pragma once
#include<unordered_set>
#include<functional>
#include<chrono>


using std::chrono::duration;
using std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::milliseconds;


class Timer;

/*----------------------------------
			  计时器接口
  ----------------------------------*/
class TimerHandler
{
	std::unordered_set<Timer*> timers;

public:
	TimerHandler() {}
	virtual ~TimerHandler();

	void AddTimer(Timer* timer) { timers.insert(timer); }
};


/*----------------------------------
			   计时器
  ----------------------------------*/
class Timer final
{
	friend class World;
public:
	Timer() { lastTime = steady_clock::now(); }
	~Timer();

	/**
	 * @brief 绑定类成员函数到计时器定时回调
	 * @param[in] delay			    计时器回调间隔
	 * @param[in] obj	            类函数所属对象指针
	 * @param[in] function          类函数指针
	 * @param[in] repeat			计时器是否可持续执行，默认情况下仅执行一次
	 * @param[in] firstDelay	    指定计时器首次执行所需时间，多用于可持续执行的计时器，若为-1则delay的值即为首次执行所需时间  
	 **/
	template<typename T>
	void Bind(double delay, T* obj, void(T::* function)(), bool repeat = false, double firstDelay = -1.0)
	{
		static_assert(std::is_base_of<TimerHandler, T>::value, "T must be a derived class of ITimerHandler");

		TimerHandler* handler = dynamic_cast<TimerHandler*>(obj);
		handler->AddTimer(this);

		callback = std::bind(function, obj);
		this->delay = duration<double>(delay);
		lastTime = steady_clock::now();
		if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (delay - firstDelay)));
		bPersistent = repeat;
		RegisterTimer();
	}

	/**
	 * @brief 绑定可调用对象到计时器定时回调(请确保函数生命周期与计时器一致)
	 * @param[in] delay			    计时器回调间隔
	 * @param[in] function          函数包装，多数情况可传入lamda函数
	 * @param[in] repeat			计时器是否可持续执行，默认情况下仅执行一次
	 * @param[in] firstDelay	    指定计时器首次执行所需时间，多用于可持续执行的计时器，若为-1则delay的值即为首次执行所需时间
	 **/
	void Bind(double delay, std::function<void()>function, bool repeat = false, double firstDelay = -1.0);

	//获取距离上一次执行的时间
	double GetDelay() { return getDelay().count(); }

	//设置执行间隔
	void SetDelay(double time) { delay = duration<double>(time); }

	//重置计时器
	void Reset() { lastTime = steady_clock::now(); }

	//暂停计时器
	void Stop() { bRunning = false; stopTime = steady_clock::now();}

	//启动计时器
	void Continue() { bRunning = true; lastTime += (steady_clock::now() - stopTime); }

private:
	std::function<void()> callback;
	bool bPersistent = false;
	bool bRunning = true;

	duration<double>delay = duration<double>(0);
	time_point<steady_clock>lastTime;
	time_point<steady_clock>stopTime;


	duration<double> getDelay()
	{
		return steady_clock::now() - lastTime;
	}

	void Execute();

	void RegisterTimer();
};