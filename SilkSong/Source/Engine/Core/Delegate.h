/**
 * @file	Delegate.h
 * @brief   事件委托类及相关宏定义，用于保存函数以便执行逻辑
 * @author	Arty
 **/

#pragma once
#include <unordered_map>
#include <functional>
#include <string>



/*----------------------------------
              单播委托
  ----------------------------------*/
template<typename R = void, typename... Args>
class UnicastDelegate
{
    std::function<R(Args...)> callback;

public:
    void Bind(std::function<R(Args...)> callback) { this->callback = callback; }

    template<typename T>
    void Bind(T* obj, void(T::* func)(Args...))
    {
        std::function<R(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
        Bind(callback);
    }

    void Unbind() { callback = nullptr; }

    bool IsBinded()const { return callback != nullptr; }

    R Execute(Args... args);

    R operator()(Args... args) { return Execute(args...); }
};


/*----------------------------------
              多播委托
  ----------------------------------*/
template<typename... Args>
class MulticastDelegate
{
    std::unordered_map<std::string, std::function<void(Args...)>> callbackMap;

public:
    bool Contains(std::string name);

    void Add(std::function<void(Args...)> callback, std::string name);

    template<typename T>
    void Add(T* obj, void(T::* func)(Args...), std::string name)
    {
        std::function<void(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
        Add(callback, name);
    }

    void Remove(std::function<void(Args...)> callback, std::string name);
              
    template<typename T>
    void Remove(T* obj, void(T::* func)(Args...), std::string name)
    {
        std::function<void(Args...)> callback = [=](Args... args) { (obj->*func)(args...); };
        Remove(callback, name);
    }

    void Clear();

    void BroadCast(Args... args);

    void operator()(Args... args) { BroadCast(args...); }
};



#define DECLARE_MULTI_PARAM_WITH_RETURN_UNICAST_DELEGATE_CLASS(Name, R, ...) typedef UnicastDelegate<R,__VA_ARGS__> Name;
#define DECLARE_NO_PARAM_WITH_RETURN_UNICAST_DELEGATE_CLASS(Name, R) typedef UnicastDelegate<R> Name;

#define DECLARE_MULTI_PARAM_UNICAST_DELEGATE_CLASS(Name, ...) typedef UnicastDelegate<void,__VA_ARGS__> Name;
#define DECLARE_NO_PARAM_UNICAST_DELEGATE_CLASS(Name) typedef UnicastDelegate<void> Name;

#define DECLARE_MULTI_PARAM_MULTICAST_DELEGATE_CLASS(Name, ...) typedef MulticastDelegate<__VA_ARGS__> Name;
#define DECLARE_NO_PARAM_MULTICAST_DELEGATE_CLASS(Name) typedef MulticastDelegate<> Name;

#define AddDynamic(obj, func) Add(obj, func, #func)
#define RemoveDynamic(obj, func) Remove(obj, func, #func)
#define AddLambda(func) Add(func, #func)
#define RemoveLambda(func) Remove(func, #func)






template<typename R, typename ...Args>
inline R UnicastDelegate<R, Args...>::Execute(Args ...args)
{
    if (callback)
    {
        return callback(args...);
    }
    return R();
}

template<typename ...Args>
inline bool MulticastDelegate<Args...>::Contains(std::string name)
{
    return callbackMap.find(name) != callbackMap.end();
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Add(std::function<void(Args...)> callback, std::string name)
{
    if (Contains(name))
    {
        return;
    }
    callbackMap.insert({ name, callback });
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Remove(std::function<void(Args...)> callback, std::string name)
{
    if (!Contains(name))
    {
        return;
    }
    callbackMap.erase(name);
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::Clear()
{
    callbackMap.clear();
}

template<typename ...Args>
inline void MulticastDelegate<Args...>::BroadCast(Args ...args)
{
    for (auto& callback : callbackMap)
    {
        callback.second(args...);
    }
}