#ifndef __MATERIALS_H__
#define __MATERIALS_H__

#include "Ray.h"

struct Hit_Record;
class Material;

struct Hit_Record {
	float t;
	Vec3 p;
	Vec3 normal;
	Material* mat_ptr;
};

class Material
{
public:
	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attentuation, Ray& scattered) const = 0;

	Vec3 albedo = Vec3(1.f, 1.f, 1.f);
};

class Lambertian : public Material
{
public:
	Lambertian(const Vec3& a)
	{
		albedo = a;
	}
	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attentuation, Ray& scattered) const
	{
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attentuation = albedo;
		return true;
	}
};

class Metal : public Material
{
public:
	Metal(const Vec3& a, float f)
	{ 
		if (f < 1) fuzz = f; else fuzz = 1;
		albedo = a;
	}

	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attentuation, Ray& scattered) const
	{
		Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere());
		attentuation = albedo;

		return (dot(scattered.direction(), rec.normal) > 0);
	}

	float fuzz;
};

#endif // !__MATERIALS_H__
