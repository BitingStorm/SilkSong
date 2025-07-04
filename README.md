# SilkSong
### 项目简介


这是一个用 EasyX 和 Windows GDI 库复刻的一款官方还在制作中的 2D 横板类银河恶魔城游戏——《丝之歌》，语言是 C++。人物素材对标《丝之歌》，而由于缺乏背景素材，背景素材用的还是《空洞骑士》。

项目分为两个部分：Engine 和 Project。Engine 部分有渲染系统、粒子系统、碰撞系统、物理模拟系统、媒体系统、动画系统、相机系统、键鼠交互系统、UI 系统、计时系统等等，整体框架借鉴了 UE 和 Unity，使用了装饰模式、组件模式、工厂模式、单例模式等等。开发者需要在场景中组织管理 Actor 游戏对象，在 Actor 游戏对象身上管理各具功能的组件。Engine 部分与游戏具体内容无关。Project 部分则是开发者需要具体编写的有关游戏内容的部分。

### 开发环境


VS2022 + EasyX_20240601 

### 素材来源


大部分美术、音效皆来自[https://www.spriters-resource.com/search/?q=Hollow+Knight](http://)或原版解包，小部分来自网上各种民间版《丝之歌》以及《空洞骑士》wiki百科网站，其余皆为作者手绘，或是用ps精修。

所有素材皆用作学习交流，严禁商用。

### 游戏简介


目前只还原了菜单场景和一个游戏场景，一种怪物和主角，但主角的所有动作、技能均已还原。

菜单“CHANGE THEME”可以在怀旧和最新场景间来回切换，游戏中 WASD 控制上下左右，按住 Space 空格疾跑，J 攻击，K 跳跃（按住时间越久跳得越高，跳跃到平台边沿会自动攀爬），F 冲刺（地面小冲刺，空中大冲刺），L 闪避，Q 飞镖（飞镖可互动，向下攻击用作垫脚石），E 治疗，I 近战技能，地面使用 O 为远程技能，空中使用 O 发射钩锁斜向上移动，U快速技能。特别的，长按 W/S 向上/下看，W+J 向上攻击，S+J 斜向下攻击，座位旁按 W 坐下，坐下以后按 S 起立，按下 X 消耗灵魂可触发一次短时间格挡，格挡成功会有回报和反击。

### 游戏执行效果

![](https://codebus.cn/f/a/0/0/734/QQ20250130-105746.png)

![](https://codebus.cn/f/a/0/0/734/QQ20250130-105855.png)

![](https://codebus.cn/f/a/0/0/734/QQ20250130-105947.png)


演示视频：[【Easyx】空洞骑士，但是Easyx](https://www.bilibili.com/video/BV1U8fSYkEsC/?share_source=copy_web&vd_source=a59fee9f89e8975f2e92a9b950cce6c5)
