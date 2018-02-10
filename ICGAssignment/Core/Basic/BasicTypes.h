#pragma once

#include "Target.h"


#ifdef TARGET_WINDOWS

typedef char					int8;
typedef unsigned char			uint8;
typedef short					int16;
typedef unsigned short			uint16;
typedef int						int32;
typedef unsigned int			uint32;
typedef long long				int64;
typedef unsigned long long		uint64;

union U32F32
{
	uint32 asUint32;
	float asFloat32;
};
union U64F64
{
	uint64 asUint64;
	double asFloat64;
};

#ifdef _WIN64
typedef long long				intPtr;
typedef unsigned long long      uintPtr;

#else
typedef int						intPtr;
typedef unsigned int			uintPtr;

#endif // _WIN64

#endif // WINDOW

// math define
namespace Constants
{
	static const float Pi = 3.1415926535897932f;
	static const float PiByTwo = 1.57079632679f;
	static const float OneOverPi = 0.31830988618f;
	static const float RAD_TO_DEG = 57.29577637f;
	static const float DEG_TO_RAD = 0.0174532935f;
	static const float EULER_NUMBER = 2.71828182845904523536f;

	static const float SMALL_NUMBER = 1.e-8f;
	static const float RELATIVE_SMALL_NUMBER = 1.e-4f;
	static const float BIG_NUMBER = 3.4e+38f;
	static const int32 MAX_NAME_LENGTH = 50;
	static const uint32 SIGN_BIT = ((1 << 31));
}
