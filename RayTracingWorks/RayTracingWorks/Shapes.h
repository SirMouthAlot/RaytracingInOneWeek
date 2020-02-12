#ifndef __SHAPES_H__
#define __SHAPES_H__

#include "Vec3.h"

#include "Materials.h"

// absstract class 
class Shape {
public:
	virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const = 0;

	Material* mat;
};


// sphere class
class Sphere : public Shape {
public:
	Sphere() {}
	Sphere(Vec3 cen, float r, Material* _mat) : center(cen), radius(r)
	{
		mat = _mat;
	};


	virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const {
		Vec3 oc = r.origin() - center;
		float a = dot(r.direction(), r.direction());
		float b = dot(oc, r.direction());
		float c = dot(oc, oc) - radius * radius;
		float discriminant = b * b - a * c;
		if (discriminant > 0) {
			float temp = (-b - sqrt(b * b - a * c)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat;
				return true;
			}
			temp = (-b + sqrt(b * b - a * c)) / a;
			if (temp < t_max && temp > t_min) {
				rec.t = temp;
				rec.p = r.point_at_parameter(rec.t);
				rec.normal = (rec.p - center) / radius;
				rec.mat_ptr = mat;
				return true;
			}
		}
		return false;
	}

	Vec3 center;
	float radius;
};


// list of objects
class Shape_List : public Shape {
public:
	Shape_List() {}
	Shape_List(Shape** l, int n) { list = l; list_size = n; }

	virtual bool hit(const Ray& r, float t_min, float t_max, Hit_Record& rec) const {
		Hit_Record temp_rec;
		bool hit_anything = false;
		double closest_so_far = t_max;
		for (int i = 0; i < list_size; i++) {
			if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}

	Shape** list;
	int list_size;
};


#endif // !__SHAPES_H__


