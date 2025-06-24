#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Quaternion.h"
#include "MathSub.h"

namespace MyMath
{
	constexpr double PI = 3.14159265358979323846;
	constexpr float PI_F = 3.14159265358979323846f;
	// 度数法を弧度法に変更
	constexpr float DEG_2_RAD = PI_F / 180.0f;
	// 弧度法を度数法に変更
	constexpr float RAD_2_DEG = 180.0f / PI_F;
}

