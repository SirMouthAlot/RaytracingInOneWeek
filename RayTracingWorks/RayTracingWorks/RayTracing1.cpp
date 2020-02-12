// RayTracing1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <float.h>

#include "Camera.h"
#include "Shapes.h"
#include "utils.h"

#include "CImg.h"

using namespace std;
using namespace cimg_library;

int max_depth = 10;
int max_samples = 32;

Vec3 color(const Ray& r, Shape* world, int depth=0) {
	Hit_Record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attentuation;
		if (depth < max_depth && rec.mat_ptr->scatter(r, rec, attentuation, scattered))
		{
			return attentuation * color(scattered, world, depth + 1);
		} else {
			return Vec3(0.f, 0.f, 0.f);
		}
	} else {
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{
	int nx, ny, countRows;
	nx = 500;
	ny = 250;

	CImg<unsigned char> img(nx, ny, 1, 3);
	CImgDisplay canvas(img, "RayTracing Test", 0);
	
	Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
	Vec3 horizontal(4.0f, 0.0f, 0.0f);
	Vec3 vertical(0.0f, 2.0f, 0.0f);
	Vec3 origin(0.0f, 0.0f, 0.0f);

	Shape* list[2];
	list[0] = new Sphere(Vec3(0, 0, -1), 0.5f, new Lambertian(Vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8f, 0.8f, 0.f)));
	//list[2] = new Sphere(Vec3(1, 0, -1), 100, new Metal(Vec3(0.8f, 0.8f, 0.f), 0.f));
	//list[3] = new Sphere(Vec3(-1, 0, -1), 100, new Metal(Vec3(0.8f, 0.8f, 0.f), 0.f));
	Shape* world = new Shape_List(list, 2);

	Camera cam;

	countRows = 0;
	for (int j = ny - 1; j >= 0; j--, countRows++) {
		for (int i = 0; i < nx; i++) {
			Vec3 col(0, 0, 0);
			for (int sample = 0; sample < max_samples; sample++)
			{
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				Ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(max_samples);

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


