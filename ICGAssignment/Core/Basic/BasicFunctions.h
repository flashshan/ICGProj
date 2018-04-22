#pragma once

#include "BasicTypes.h"
#include "Assert.h"

#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>	 
#include <vector>

namespace Basic {
	template<typename T> FORCEINLINE void Swap(T &i_Left, T &i_Right);

	// reverse between small end and big end machine
	FORCEINLINE int16 SwapInt16(int16 i_value);
	FORCEINLINE uint16 SwapUint16(uint16 i_value);

	FORCEINLINE int32 SwapInt32(int32 i_value);
	FORCEINLINE uint32 SwapUint32(uint32 i_value);

	FORCEINLINE int64 SwapInt64(int64 i_value);
	FORCEINLINE uint64 SwapUint64(uint64 i_value);

	FORCEINLINE float SwapFloat32(float i_value);
	FORCEINLINE double swapFloat64(double i_value);
}


namespace Float {

	FORCEINLINE bool IsNAN(float i_val);
	FORCEINLINE bool IsZero(float i_val);
	FORCEINLINE bool IsFinite(float i_val);

	// fastest
	FORCEINLINE bool EqualFast(float i_lhs, float i_rhs, float i_maxDiff = Constants::SMALL_NUMBER);
	// balanced
	FORCEINLINE bool EqualRelated(float i_lhs, float i_rhs, float i_maxDiff = Constants::SMALL_NUMBER);
	// slow but sure
	FORCEINLINE bool EqualAccurate(float i_lhs, float i_rhs, float i_maxDiff, uint32 i_maxULPS = 12);

	static const uint32 allBitsSet = ~0;
}

struct Rotator;
struct VectorSSE;
struct Quaternion;

namespace Math {

	template<typename T> FORCEINLINE T Max(const T &i_value1, const T&i_value2);
	template<typename T> FORCEINLINE T Min(const T &i_value1, const T&i_value2);
	template<typename T> FORCEINLINE T Clamp(const T& i_value, const T& i_min, const T& i_max);
	template<typename T> inline float Average(const std::vector<T>& i_vector);
	inline float StandardDeviation(const std::vector<float>& v);


	FORCEINLINE float DegreesToRadians(float i_Degrees);
	FORCEINLINE float RadiansToDegrees(float i_Radians);

	FORCEINLINE float Fmod(float X, float Y);

	FORCEINLINE int Abs(int i_value);
	FORCEINLINE float AbsF(float i_float);

	FORCEINLINE void RandInit(uint32 i_seed) { srand(i_seed); }

	FORCEINLINE int32 Rand() { return rand(); }	
	FORCEINLINE int32 RandInt(int32 i_bound);
	FORCEINLINE int32 RandInt(int32 i_lowerBound, int32 i_upperBound);
	FORCEINLINE uint32 RandUInt(uint32 i_bound);
	FORCEINLINE uint32 RandUInt(uint32 i_lowerBound, uint32 i_upperBound);

	FORCEINLINE float RandOne();
	FORCEINLINE float RandFloat(float i_lowerBound, float i_upperBound);
	FORCEINLINE bool RandBool() { return rand() % 2 == 1; }

	//returns a random number with a normal distribution. See method at //http://www.taygeta.com/random/gaussian.html
	inline float RandGaussian(float i_mean = 0.0f, float i_standardDeviation = 1.0f);

	// from Unreal
	FORCEINLINE float FastAsin(float i_value);

	FORCEINLINE void SinCos(float* o_scalarSin, float* o_scalarCos, float i_rad);
	FORCEINLINE void VectorSinCos(VectorSSE* o_sinAngles, VectorSSE* o_cosAngles, const VectorSSE* i_angles);
	FORCEINLINE float Sin(float i_rad) { return sinf(i_rad); }
	FORCEINLINE float Cos(float i_rad) { return cosf(i_rad); }
	FORCEINLINE float Tan(float i_rad) { return tanf(i_rad); }

	FORCEINLINE Rotator Lerp(const Rotator& i_rotator1, const Rotator& i_rotator2, float i_alpha);
	FORCEINLINE Rotator LerpRange(const Rotator& i_rotator1, const Rotator& i_rotator2, float i_alpha);
	
	FORCEINLINE Quaternion Lerp(const Quaternion &i_quat1, const Quaternion &i_quat2, float i_alpha);
	FORCEINLINE Quaternion BiLerp(const Quaternion& i_quat00, const Quaternion& i_quat10, const Quaternion& i_quat01, const Quaternion& i_quat11, float i_fracX, float i_fracY);
	FORCEINLINE Quaternion CubicInterp(const Quaternion& i_quatP0, const Quaternion& i_quatT0, const Quaternion& i_quatP1, const Quaternion& i_quatT1, float i_alpha);


	// from Unreal
	FORCEINLINE float InvSqrt(float i_float);
	FORCEINLINE float InvSqrtEst(float i_float);

}





// implement forceinline

namespace Basic {

	//template<typename T> FORCEINLINE void Swap(T &i_Left, T &i_Right)
	//{
	//	T temp = i_Left;
	//	i_Left = i_Right;
	//	i_Right = temp;
	//}

	template<typename T> FORCEINLINE void Swap(T &i_Left, T &i_Right)
	{
		std::swap(i_Left, i_Right);
	}

	FORCEINLINE int16 SwapInt16(int16 i_value)
	{
		return ((i_value & 0x00ff) << 8)
			| ((i_value & 0xff00) >> 8);
	}
	FORCEINLINE uint16 SwapUint16(uint16 i_value)
	{
		return ((i_value & 0x00ff) << 8)
			| ((i_value & 0xff00) >> 8);
	}

	FORCEINLINE int32 SwapInt32(int32 i_value)
	{
		return ((i_value & 0x000000ff) << 24)
			| ((i_value & 0x0000ff00) << 8)
			| ((i_value & 0x00ff0000) >> 8)
			| ((i_value & 0xff000000) >> 24);
	}
	FORCEINLINE uint32 SwapUint32(uint32 i_value)
	{
		return ((i_value & 0x000000ff) << 24)
			| ((i_value & 0x0000ff00) << 8)
			| ((i_value & 0x00ff0000) >> 8)
			| ((i_value & 0xff000000) >> 24);
	}

	FORCEINLINE int64 SwapInt64(int64 i_value)
	{
		return ((i_value & 0x00000000000000ff) << 56)
			| ((i_value & 0x000000000000ff00) << 40)
			| ((i_value & 0x0000000000ff0000) << 24)
			| ((i_value & 0x00000000ff000000) << 8)
			| ((i_value & 0x000000ff00000000) >> 8)
			| ((i_value & 0x0000ff0000000000) >> 24)
			| ((i_value & 0x00ff000000000000) >> 40)
			| ((i_value & 0xff00000000000000) >> 56);
	}
	FORCEINLINE uint64 SwapUint64(uint64 i_value)
	{
		return ((i_value & 0x00000000000000ff) << 56)
			| ((i_value & 0x000000000000ff00) << 40)
			| ((i_value & 0x0000000000ff0000) << 24)
			| ((i_value & 0x00000000ff000000) << 8)
			| ((i_value & 0x000000ff00000000) >> 8)
			| ((i_value & 0x0000ff0000000000) >> 24)
			| ((i_value & 0x00ff000000000000) >> 40)
			| ((i_value & 0xff00000000000000) >> 56);
	}

	FORCEINLINE float SwapFloat32(float i_value)
	{
		U32F32 temp;
		temp.asFloat32 = i_value;
		temp.asUint32 = SwapUint32(temp.asUint32);
		return temp.asFloat32;
	}
	FORCEINLINE double swapFloat64(double i_value)
	{
		U64F64 temp;
		temp.asFloat64 = i_value;
		temp.asUint64 = SwapUint64(temp.asUint64);
		return temp.asFloat64;
	}
}


namespace Float {

	FORCEINLINE bool IsNAN(float i_val)
	{
		/*volatile float val = i_val;
		return val != val;*/

		// another way to judge
		return ((*(uint32*)&i_val) & 0x7FFFFFFF) > 0x7F800000;
	}

	FORCEINLINE bool IsZero(float i_val)
	{
		return EqualFast(i_val, static_cast<float>(Constants::SMALL_NUMBER));
	}
	FORCEINLINE bool IsFinite(float i_val)
	{
		return ((*(uint32*)&i_val) & 0x7F800000) != 0x7F800000;
	}


	FORCEINLINE bool EqualFast(float i_lhs, float i_rhs, float i_maxDiff)
	{
		return fabs(i_lhs - i_rhs) < i_maxDiff;
	}

	FORCEINLINE bool EqualRelated(float i_lhs, float i_rhs, float i_maxDiff)
	{
		if (i_lhs == i_rhs)
			return true;

		float relDiff;

		if (fabs(i_rhs) > fabs(i_lhs))
			relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_rhs));
		else
			relDiff = static_cast<float>(fabs((i_lhs - i_rhs) / i_lhs));

		return relDiff <= i_maxDiff;
	}

	// slow but sure
	FORCEINLINE bool EqualAccurate(float i_lhs, float i_rhs, float i_maxDiff, uint32 i_maxULPS)
	{
		ASSERT(sizeof(float) == sizeof(int32));

		if (i_lhs == i_rhs)
			return true;

		float diff = static_cast<float>(fabs(i_lhs - i_rhs));
		if (diff <= i_maxDiff)
			return true;

		uint32 intDiff = abs(*reinterpret_cast<int *>(&i_lhs) - *reinterpret_cast<int *>(&i_rhs));
		return intDiff <= i_maxULPS;
	}
}



namespace Math {

	template<typename T> FORCEINLINE T Max(const T &i_value1, const T&i_value2)
	{
		return (i_value1 < i_value2) ? i_value2 : i_value1;
	}

	template<typename T> FORCEINLINE T Min(const T &i_value1, const T&i_value2)
	{
		return (i_value1 < i_value2) ? i_value1 : i_value2;
	}

	template<typename T> FORCEINLINE T Clamp(const T& i_value, const T& i_min, const T& i_max)
	{
		return i_value < i_min ? i_min : i_value < i_max ? i_value : i_max;
	}

	template<typename T> inline float Average(const std::vector<T>& i_vector)
	{
		float average = 0.0f;

		for (unsigned int i = 0; i < i_vector.size(); ++i)
		{
			average += static_cast<float>(i_vector[i]);
		}

		return average / static_cast<float>(i_vector.size());
	}

	inline float StandardDeviation(const std::vector<float>& i_vector)
	{
		float sd = 0.0;
		float average = Average(i_vector);

		for (unsigned int i = 0; i < i_vector.size(); ++i)
		{
			sd += (i_vector[i] - average) * (i_vector[i] - average);
		}

		return sqrtf(sd / static_cast<float>(i_vector.size()));
	}


	FORCEINLINE float DegreesToRadians(float i_Degrees)
	{
		return i_Degrees * Constants::DEG_TO_RAD;
	}

	FORCEINLINE float RadiansToDegrees(float i_Radians)
	{
		return i_Radians * Constants::RAD_TO_DEG;
	}

	// from Unreal
	FORCEINLINE float Fmod(float X, float Y)
	{
		ASSERT(Math::AbsF(Y) > Constants::SMALL_NUMBER);

		const float Quotient = static_cast<float>(static_cast<int32>(X / Y));
		float IntPortion = Y * Quotient;

		// Rounding and imprecision could cause IntPortion to exceed X and cause the result to be outside the expected range.
		// For example Fmod(55.8, 9.3) would result in a very small negative value!
		if (Math::AbsF(IntPortion) > Math::AbsF(X))
		{
			IntPortion = X;
		}

		const float Result = X - IntPortion;
		return Result;
	}

	FORCEINLINE int Abs(int i_value)
	{
		return i_value > 0 ? i_value : -i_value;
	}

	FORCEINLINE float AbsF(float i_float)
	{
		return i_float > 0 ? i_float : -i_float;
	}

	FORCEINLINE int32 RandInt(int32 i_bound)
	{
		ASSERT(i_bound > 0);
		return rand() % i_bound;
	}
	FORCEINLINE int32 RandInt(int32 i_lowerBound, int32 i_upperBound)
	{
		ASSERT(i_lowerBound < i_upperBound);
		return i_lowerBound + rand() % (i_upperBound - i_lowerBound);
	}
	FORCEINLINE uint32 RandUInt(uint32 i_bound)
	{
		ASSERT(i_bound != 0);
		return static_cast<uint32>(rand()) % i_bound;
	}
	FORCEINLINE uint32 RandUInt(uint32 i_lowerBound, uint32 i_upperBound)
	{
		ASSERT(i_lowerBound < i_upperBound);
		return i_lowerBound + static_cast<uint32>(rand()) % (i_upperBound - i_lowerBound);
	}
	FORCEINLINE float RandOne()
	{
		return static_cast<float>(rand()) / static_cast<float>(RAND_MAX + 1);
	}
	FORCEINLINE float RandFloat(float i_lowerBound, float i_upperBound)
	{
		ASSERT(i_lowerBound < i_upperBound);
		return i_lowerBound + RandOne() * (i_upperBound - i_lowerBound);
	}

	inline float RandGaussian(float i_mean, float i_standardDeviation)
	{
		float x1, x2, w, y1;
		static float y2;
		static int use_last = 0;

		if (use_last)		        /* use value from previous call */
		{
			y1 = y2;
			use_last = 0;
		}
		else
		{
			do
			{
				x1 = 2.0f * RandOne() - 1.0f;
				x2 = 2.0f * RandOne() - 1.0f;
				w = x1 * x1 + x2 * x2;
			} while (w >= 1.0f);

			w = sqrtf((-2.0f * logf(w)) / w);
			y1 = x1 * w;
			y2 = x2 * w;
			use_last = 1;
		}

		return(i_mean + y1 * i_standardDeviation);
	}

	// modified from UE4
	FORCEINLINE float FastAsin(float i_value)
	{
		// Clamp input to [-1,1].
		float x = Math::AbsF(i_value);
		float omx = 1.0f - x;
		omx = (omx < 0.f) ? 0.f : omx;

		float root = sqrtf(omx);
		// 7-degree minimax approximation
		float result = ((((((-0.0012624911f * x + 0.0066700901f) * x - 0.0170881256f) * x + 0.0308918810f) * x - 0.0501743046f) * x + 0.0889789874f) * x - 0.2145988016f) * x + Constants::PiByTwo;
		result *= root;  // acos(|x|)
						 // acos(x) = pi - acos(-x) when x < 0, asin(x) = pi/2 - acos(x)
		return ((i_value >= 0.0f) ? Constants::PiByTwo - result : result - Constants::PiByTwo);
	}

	// modified from UE4
	FORCEINLINE void SinCos(float* o_scalarSin, float* o_scalarCos, float i_rad)
	{
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (Constants::OneOverPi*0.5f)*i_rad;
		if (i_rad >= 0.0f)
		{
			quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
		}
		else
		{
			quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
		}
		float y = i_rad - (2.0f*Constants::Pi)*quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign;
		if (y > Constants::PiByTwo)
		{
			y = Constants::Pi - y;
			sign = -1.0f;
		}
		else if (y < -Constants::PiByTwo)
		{
			y = -Constants::Pi - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		*o_scalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		*o_scalarCos = sign*p;
	}

	// modified from UE4
	FORCEINLINE float InvSqrt(float i_float)
	{
		// Performs two passes of Newton-Raphson iteration on the hardware estimate
		//    v^-0.5 = x
		// => x^2 = v^-1
		// => 1/(x^2) = v
		// => F(x) = x^-2 - v
		//    F'(x) = -2x^-3

		//    x1 = x0 - F(x0)/F'(x0)
		// => x1 = x0 + 0.5 * (x0^-2 - Vec) * x0^3
		// => x1 = x0 + 0.5 * (x0 - Vec * x0^3)
		// => x1 = x0 + x0 * (0.5 - 0.5 * Vec * x0^2)
		//
		// This final form has one more operation than the legacy factorization (X1 = 0.5*X0*(3-(Y*X0)*X0)
		// but retains better accuracy (namely InvSqrt(1) = 1 exactly).

		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(i_float);
		X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		// 1st Newton-Raphson iteration
		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		// 2nd Newton-Raphson iteration
		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}

	// modified from UE4
	FORCEINLINE float InvSqrtEst(float i_float)
	{
		// Performs one pass of Newton-Raphson iteration on the hardware estimate
		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, FOver2;
		float temp;

		Y0 = _mm_set_ss(i_float);
		X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		// 1st Newton-Raphson iteration
		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		_mm_store_ss(&temp, X1);
		return temp;
	}
}