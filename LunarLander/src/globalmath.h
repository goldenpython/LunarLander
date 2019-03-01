#pragma once

#define MATH_INLINE		__forceinline

template <typename T>
static T Clamp(T tValue, T tMin, T tMax) {
	if (tValue < tMin)
		return tMin;
	if (tValue > tMax)
		return tMax;
	return tValue;
}

template <typename T>
static T Det3(T tX1, T tY1, T tX2, T tY2, T tX3, T tY3) {
	return (tX1 * (tY2 - tY3) + tX2 * (tY3 - tY1) + tX3 * (tY1 - tY2));
}

template <typename T>
static int Sgn(T tNumber) {
	return tNumber > 0 ? 1 : -1;
}

const float G = 0.15f;
const float PI = 3.1415f;
const float POWER = G * 1.4f;
const float ANGULAR_POWER = PI / 180 * 25; // 3.1415f / 180 * 15;
const float	ANGULAR_SPEED_DECAY = 0.8f;
const float MAX_LANDING_SPEED_POW2 = (G * 2) * (G * 2);
const float MAX_COS_ANGLE_LANDING_DIFF = 1E-2f;
const float MAX_WIND_SPEED = G * 35; //G * 2;
const float DAMAGE_BY_SPEED = G * 3;
const float MIN_VALUE = 1E-2f;
