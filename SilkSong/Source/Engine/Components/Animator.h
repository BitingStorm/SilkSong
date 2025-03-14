/**
 * @file	Animator.h
 * @brief	包含所有与动画相关内容的管理逻辑
 * @author	Arty
 **/




#pragma once
#include"ActorComponent.h"





/* 动画委托 */
DECLARE_NO_PARAM_UNICAST_DELEGATE_CLASS(AnimationDelegate)


class IMAGE;
class Animator;
class AnimEdge;
class ImageInterface;



/*----------------------------------
			   动画源
  ----------------------------------*/
class Animation final :public TimerHandler
{
    friend Animator;
	friend AnimEdge;

	Animator* animController = nullptr;
	int32 num = 0;//动画帧数
	FVector2D offset = {0,0};//偏移量
	IMAGE** images = nullptr;//动画帧数组
	int32 index = 0;//当前帧索引
	bool bLooping = true;//是否可循环播放
	bool bReverse = false;//是否反向播放
	bool bMontage = false;//是否处于蒙太奇播放

	Timer clock;//计时器

	std::unordered_map<int32, AnimationDelegate>notifications;//自定义动画通知

	std::list<AnimEdge*>nexts;//动画桥

	void Tick();

public:
	Animation() { clock.Bind(0, this, &Animation::Tick, true); clock.Stop();}

	//加载动画资源
	void Load(std::string name, FVector2D delta = {0,0});

	//设置动画帧间隔时间
	void SetInterval(double interval) { clock.SetDelay(interval); }

	//设置动画帧下标
	void SetIndex(int32 i) { index = i; }

	//获取动画帧下标
	int32 GetIndex()const { return index; }

	//设置是否循环播放
	void SetLooping(bool loop) { bLooping = loop; }

	//在指定帧处添加动画通知
	void AddNotification(int32 index, const AnimationDelegate& event) { notifications.insert({ index,event }); }

	//获取是否反向播放
	bool IsReverse()const { return bReverse; }

	//设置是否反向播放
	void SetReverse(bool bIsReverse) { bReverse = bIsReverse; }

	AnimationDelegate OnAnimEnter;//进入动画事件
	AnimationDelegate OnAnimExit;//离开动画事件

private:
	AnimationDelegate OnMontageExit;//蒙太奇结束事件（用于记录播放蒙太奇前一个动画，以便于当蒙太奇无后继节点时触发该事件从而恢复前一个动画）

	bool exitLock = false;//动画结束播放锁，确保上一个动画节点OnAnimExit只执行一次，防止无限递归
};







/* 转换条件比较 */
enum class TransitionComparison : uint8
{
	Equal,
	NotEqual,
	Greater,
	Less,
	GreaterEqual,
	LessEqual
};


/* 动画转换条件 */
namespace AnimTransition
{
	struct Integer 
	{
		std::string paramName;
		int32 value;
		TransitionComparison comparison;
	};

	struct Float 
	{
		std::string paramName;
		float value;
		TransitionComparison comparison;
	};

	struct Bool 
	{
		std::string paramName;
		bool value;
	};

	struct Trigger
	{
		std::string paramName;
	};

	enum ComparisonMode
	{
		AND,OR
	};

	template<typename T>
	bool GetComparisonResult(T a, T b, TransitionComparison comp)
	{
		switch (comp) {
		case TransitionComparison::Equal:return a == b;
		case TransitionComparison::Greater:return a > b;
		case TransitionComparison::GreaterEqual:return a >= b;
		case TransitionComparison::Less:return a < b;
		case TransitionComparison::LessEqual:return a <= b;
		case TransitionComparison::NotEqual:return a != b;
		}
		return false;
	}
}


/*----------------------------------
			动画过渡(有向边)
  ----------------------------------*/
class AnimEdge final
{
	friend Animator;
	friend void Animation::Tick();

	Animation* start;
	Animation* end;

	std::vector<AnimTransition::Integer> integerConditions;
	std::vector<AnimTransition::Float> floatConditions;
	std::vector<AnimTransition::Bool> boolConditions;
	std::vector<AnimTransition::Trigger> triggerConditions;

	AnimTransition::ComparisonMode comparisonMode;

public:
	AnimEdge() :start{}, end{}, comparisonMode(AnimTransition::ComparisonMode::AND) {}

	/**
	 * @brief 动画过渡初始化
	 * @param[in] start			    出发节点
	 * @param[in] end	            到达节点
	 * @param[in] mode              过渡比较模式（AND需要满足所有条件，OR只需满足任意条件）
	 **/
	void Init(Animation& start, Animation& end, AnimTransition::ComparisonMode mode = AnimTransition::AND)
	{
		this->start = &start; this->end = &end; comparisonMode = mode; this->start->nexts.push_back(this);
	}

	//添加过渡条件
	void AddCondition(const AnimTransition::Integer& condition) 
	{
		integerConditions.push_back(condition);
    }
	void AddCondition(const AnimTransition::Float& condition)
	{
	    floatConditions.push_back(condition);
	}
	void AddCondition(const AnimTransition::Bool& condition)
	{
		boolConditions.push_back(condition);
	}
	void AddCondition(const AnimTransition::Trigger& condition)
	{
		triggerConditions.push_back(condition);
	}

	//是否无过渡条件
	bool IsUnconditional()const 
	{ 
		return integerConditions.empty() && floatConditions.empty() && boolConditions.empty() && triggerConditions.empty();
	}
};








/* 状态参数类型 */
enum class ParamType: uint8
{
	Integer, Bool, Float, Trigger
};


/*----------------------------------
			  动画播放器
  ----------------------------------*/
class Animator :public ActorComponent
{
	DEFINE_SUPER(ActorComponent)

    friend ImageInterface;
public:
	virtual ~Animator();
	virtual void BeginPlay() override;
	virtual void Update(float deltaTime) override;

	virtual void Activate()override;
	virtual void Deactivate()override;

	//插入动画节点
	void Insert(std::string name, Animation& ani);

	//设置动画节点(如果不熟悉Animator架构不建议直接使用)
	void SetNode(std::string nodeName);

	//设置动画节点(如果不熟悉Animator架构不建议直接使用)
	void SetNode(Animation* node);

	//是否在播放某动画
	bool IsPlaying(std::string nodeName);

	//设置附着渲染器
	void SetupAttachment(ImageInterface* renderer);

	//播放动画蒙太奇片段
	void PlayMontage(std::string nodeName);

	//添加参数
	void AddParamater(std::string paramName, ParamType type);

	//设置int参数
	void SetInteger(std::string paramName, int32 value);

	//设置float参数
	void SetFloat(std::string paramName, float value);

	//设置bool参数
	void SetBool(std::string paramName, bool value);

	//启动触发器
	void SetTrigger(std::string paramName);

	//获取int参数
	int32 GetInteger(std::string paramName);

	//获取float参数
	float GetFloat(std::string paramName);

	//获取bool参数
	bool GetBool(std::string paramName);

	//是否符合过渡条件
	bool CheckConditions(AnimEdge* edge);

	//获取当前播放速度
	float GetStep()const { return step; }

	//设置播放速度
	void SetStep(float value) { step = value; }

private:
	std::unordered_map<std::string, Animation&> animations;

	std::unordered_map<std::string, int> paramatersInteger;
	std::unordered_map<std::string, float> paramatersFloat;
	std::unordered_map<std::string, bool> paramatersBool;
	std::unordered_map<std::string, bool> paramatersTrigger;

	Animation* aniNode = nullptr;//当前播放的动画

	Animation* lastNode = nullptr;//播放蒙太奇前的动画

	ImageInterface* rendererAttached = nullptr;//附着的渲染器

	IMAGE* currentSprite = nullptr;//当前播放的图像

	float step = 1.f;//播放速度
};