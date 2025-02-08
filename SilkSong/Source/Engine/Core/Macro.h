/**
 * @file	Macro.h
 * @brief	别名、宏定义
 * @author	Arty
 **/

#pragma once


typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;

#define DEFINE_SUPER(SuperClass) typedef SuperClass Super;
#define TO_STRING(T) #T
#define GET_A_VALUE(rgba)				(BYTE)( ((rgba)>>24) & 0xFF )
#define SET_ALPHA(rgb, a)				(COLORREF)( ((rgb) & 0xFFFFFF) | ((a)<<24) )
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#ifdef  _MSC_VER
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE inline
#endif


constexpr int WIN_WIDTH = 1200;
constexpr int WIN_HEIGHT = 800;