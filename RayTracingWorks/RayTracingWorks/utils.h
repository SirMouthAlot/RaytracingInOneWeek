#pragma once
#include "vec3.h"
#define M_PI 3.14159265358979323846264338327950288
#include <cstdlib>

inline double random_double() {
	return rand() / (RAND_MAX + 1.0);
}

Vec3 random_in_unit_disk() {
	Vec3 p;
	do {
		p = 2.0f * Vec3(random_double(), random_double(), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

Vec3 random_in_unit_sphere()
{
	Vec3 p;
	do {
		p = 2.0 * Vec3(random_double(), random_double(), random_double()) - Vec3(1.0, 1.0, 1.0);

	} while (p.squared_length() >= 1.0);
	return p;
}

Vec3 reflect(const Vec3& v, const Vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

