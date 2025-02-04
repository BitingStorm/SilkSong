/**
 * @file	Macro.h
 * @brief	�������궨��
 * @author	Arty
 **/

#pragma once



constexpr int WIN_WIDTH = 1200;
constexpr int WIN_HEIGHT = 800;

typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;

#define DEFINE_SUPER(SuperClass) typedef SuperClass Super;
#define To_String(x) #x;
#define GetAValue(rgba)					(BYTE)( ((rgba)>>24) & 0xFF )
#define SET_ALPHA(rgb, a)				(COLORREF)( ((rgb) & 0xFFFFFF) | ((a)<<24) )
#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))