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
};

class Lambertian : public Material
{
public:
	Lambertian(const Vec3& a) : albedo(a) {}
	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attentuation, Ray& scattered) const
	{
		Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
		scattered = Ray(rec.p, target - rec.p);
		attentuation = albedo;
		return true;
	}

	Vec3 albedo = Vec3(1.f, 1.f, 1.f);
};

class Metal : public Material {
public:
	Metal(const Vec3& a, float f=0.f) : albedo(a) 
	{
		if (f < 1) fuzz = f; else fuzz = 1;
	}

	virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attenuation, Ray& scattered) const 
	{
		Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}

	float fuzz;
	Vec3 albedo;
};

class Dielectric : public Material {
public:
    Dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const Ray& r_in, const Hit_Record& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 outward_normal;
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);
        Vec3 refracted;

        float reflect_prob;
        float cosine;

        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal)
                / r_in.direction().length();
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal)
                / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else {
            reflect_prob = 1.0;
        }

        if (random_double() < reflect_prob) {
            scattered = Ray(rec.p, reflected);
        }
        else {
            scattered = Ray(rec.p, refracted);
        }

        return true;
    }

    float ref_idx;
};

#endif // !__MATERIALS_H__
