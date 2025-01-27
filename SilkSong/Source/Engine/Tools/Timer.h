/**
 * @file	Timer.h
 * @brief   ��ʱ���ඨ�壬�ṩ��ȷ�ļ�ʱ���ص������߼�
 * @author	Arty
 **/

#pragma once
#include"Core/World.h"
#include<functional>
#include<chrono>


using std::chrono::duration;
using std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::milliseconds;


/*----------------------------------
			  ��ʱ��
  ----------------------------------*/
class Timer final
{
	friend class World;
public:
	Timer() { lastTime = steady_clock::now(); }
	~Timer() { mainWorld.GameTimers.erase(this); }

	/**
	 * @brief �����Ա��������ʱ����ʱ�ص�
	 * @param[in] delay			    ��ʱ���ص����
	 * @param[in] obj	            �ຯ����������ָ��
	 * @param[in] function          �ຯ��ָ��
	 * @param[in] repeat			��ʱ���Ƿ�ɳ���ִ�У�Ĭ������½�ִ��һ��
	 * @param[in] firstDelay	    ָ����ʱ���״�ִ������ʱ�䣬�����ڿɳ���ִ�еļ�ʱ������Ϊ-1��delay��ֵ��Ϊ�״�ִ������ʱ��  
	 **/
	template<typename T>
	void Bind(double delay, T* obj, void(T::* function)(), bool repeat = false, double firstDelay = -1.0)
	{
		static_assert(std::is_base_of<ITimerHandler, T>::value, "T must be a derived class of ITimerHandler");

		ITimerHandler* handler = Cast<ITimerHandler>(obj);
		handler->AddTimer(this);

		callback = std::bind(function, obj);
		this->delay = duration<double>(delay);
		lastTime = steady_clock::now();
		if (firstDelay >= 0)lastTime -= milliseconds(int(1000 * (delay - firstDelay)));
		bPersistent = repeat;
		mainWorld.GameTimers.insert(this);
	}

	/**
	 * @brief �󶨿ɵ��ö��󵽼�ʱ����ʱ�ص�(��ȷ�����������������ʱ��һ��)
	 * @param[in] delay			    ��ʱ���ص����
	 * @param[in] function          ������װ����������ɴ���lamda����
	 * @param[in] repeat			��ʱ���Ƿ�ɳ���ִ�У�Ĭ������½�ִ��һ��
	 * @param[in] firstDelay	    ָ����ʱ���״�ִ������ʱ�䣬�����ڿɳ���ִ�еļ�ʱ������Ϊ-1��delay��ֵ��Ϊ�״�ִ������ʱ��
	 **/
	void Bind(double delay, std::function<void()>function, bool repeat = false, double firstDelay = -1.0);

	//��ȡ������һ��ִ�е�ʱ��
	double GetDelay() { return getDelay().count(); }

	//����ִ�м��
	void SetDelay(double time) { delay = duration<double>(time); }

	//���ü�ʱ��
	void Reset() { lastTime = steady_clock::now(); }

	//��ͣ��ʱ��
	void Stop() { bRunning = false; stopTime = steady_clock::now();}

	//������ʱ��
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
};