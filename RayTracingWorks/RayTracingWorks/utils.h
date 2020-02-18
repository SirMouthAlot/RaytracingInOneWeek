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

bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) 
{
	Vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	} else {
		return false;
	}
}

float schlick(float cosine, float ref_idx) 
{
	float r0 = (1 - ref_idx) / (1 + ref_idx);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}
