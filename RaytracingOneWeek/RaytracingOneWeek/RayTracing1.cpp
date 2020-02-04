// RayTracing1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <float.h>
#include <vector>

#include "CImg.h"
#include "vec3.h"
#include "utils.h"
#include "ray.h"

using namespace std;
using namespace cimg_library;

vec3 color(ray& r, Util::hittable* world) {
	Util::hit_record rec;
	if (world->hit(r, 0.f, FLT_MAX, rec))
	{
		vec3 N = rec.normal;
		return 0.5f * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}

	vec3 unit_dir = unit_vector(r.direction());
	float t = 0.5f * (unit_dir.y() + 1.0f);
	return (1.0 - t) * vec3(1,1,1) + t * vec3(0.5f, 0.7f, 1.0f);
}
int main()
{
	int nx, ny, countRows;
	nx = 500;
	ny = 250;


	CImg<unsigned char> img(nx, ny, 1, 3);
	CImgDisplay canvas(img, "RayTracing Test2", 0);

	vec3 origin(0, 0, 0);
	vec3 lower_left_corner(-2, -1, -1);
	vec3 horizontal(4, 0, 0);
	vec3 vertical(0, 2, 0);

	Util::hittable* list[3];
	list[0] = new Util::Sphere(vec3(0, 0, -1.f), 0.5f);
	list[1] = new Util::Sphere(vec3(0.5f, 0.f, -1.f), 0.2f);
	list[2] = new Util::Sphere(vec3(0, -51, -1), 50);
	Util::hittable* world = new Util::hittable_list(list, 3);


	countRows = 0;
	for (int j = ny - 1; j >= 0; j--, countRows++) {
		for (int i = 0; i < nx; i++) {
			float u, v;
			u = float(i)/float(nx);
			v = float(j)/float(ny);

			ray r(origin, lower_left_corner + u*horizontal + v*vertical);
			vec3 col = color(r, world);
			int ir = int(255.99f * col[0]);
			int ig = int(255.99f * col[1]);
			int ib = int(255.99f * col[2]);

			img(i, j, 0, 0) = ir;
			img(i, j, 0, 1) = ig;
			img(i, j, 0, 2) = ib;
		}
		float percentDone = 100.f * (float(countRows) / float(ny - 1));
		cout << percentDone << "%" << endl;
	}
	canvas.resize(nx, ny);
	img.mirror('y');
	while (!canvas.is_closed() && !canvas.is_keyESC()) {
		img.display(canvas);
		cimg::wait(20);
	}
}


