// RayTracing1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Timer.h"

#include <iostream>
#include <float.h>
#include <vector>
#include <thread>
#include <string>

#include "Camera.h"
#include "Shapes.h"
#include "utils.h"

#include "CImg.h"

using namespace std;
using namespace cimg_library;

int max_depth = 5;
int max_samples = 32;

//Uses rays and the "world" to get colors
Vec3 color(const Ray& r, Shape* world, int depth = 0) {
	Hit_Record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return Vec3(0, 0, 0);
		}
	}
	else {
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f * (unit_direction.y() + 1.0f);
		return (1.0f - t) * Vec3(1.0f, 1.0f, 1.0f) + t * Vec3(0.5f, 0.7f, 1.0f);
	}
}

//Raytraces entire screen
void RayTrace(int nx, int ny, Shape* world, CImg<unsigned char>& img, Camera& cam)
{
	int countRows = 0;
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
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

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
}

//Raytraces a specific area of the screen
void RayTraceArea(int nxMin, int nyMin, int nxMax, int nyMax, int nxActual, int nyActual, Shape* world, CImg<unsigned char>& img, Camera& cam, CImgDisplay& canvas)
{
	int countRows = 0;
	for (int j = (nyMax)-1; j >= nyMin; j--, countRows++) {
		for (int i = nxMin; i < (nxMax); i++) {
			Vec3 col(0, 0, 0);
			for (int sample = 0; sample < max_samples; sample++)
			{
				float u = float(i + random_double()) / float(nxActual);
				float v = float(j + random_double()) / float(nyActual);
				Ray r = cam.get_ray(u, v);
				col += color(r, world);
			}
			col /= float(max_samples);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			int ir = int(255.99f * col[0]);
			int ig = int(255.99f * col[1]);
			int ib = int(255.99f * col[2]);

			img(i, j, 0, 0) = ir;
			img(i, j, 0, 1) = ig;
			img(i, j, 0, 2) = ib;
		}
		float percentDone = 100.f * (float(countRows) / float((nyMax - nyMin) - 1));
		cout << percentDone << "%" << endl;
	}

	canvas.display(img);
}

//Speeds up raytracing by segmenting the tracing into different threads that happen in parallel
void RayTraceFaster(int power, int nx, int ny, Shape* world, CImg<unsigned char>& img, Camera& cam, CImgDisplay& canvas)
{
	std::vector<std::thread> threads;

	if (power <= 0)
	{
		throw std::runtime_error("Get outta here with your negative powers and divide by zero shit");
	}


	{
		for (int i = 0; i < power; i++)
		{
			for (int j = 0; j < power; j++)
			{
				//RayTraceArea(((j * nx) / power), ((i * ny) / power), (((j + 1) * nx) / power), (((i + 1) * ny) / power), nx, ny, std::ref(world), std::ref(img), std::ref(cam), std::ref(canvas));
				threads.push_back(std::thread(RayTraceArea, ((j * nx) / power), ((i * ny) / power), (((j + 1) * nx) / power), (((i + 1) * ny) / power), nx, ny, std::ref(world), std::ref(img), std::ref(cam), std::ref(canvas)));
			}
		}
	}

	//Join threads at end
	for (int i = 0; i < threads.size(); i++)
	{
		threads[i].join();
	}
}

void TrackTimePassed()
{
	while (true)
	{
		Timer::Update();
	}
}

int main()
{
	try {
		std::string input;
		std::cout << "Please enter a number to divide the screen by for speed up: ";
		std::getline(std::cin, input);
		int square = std::atoi(input.c_str());

		std::thread timerThread = std::thread(TrackTimePassed);
		timerThread.detach();
		Timer::Reset();

		int nx, ny, countRows;
		nx = 1000;
		ny = 800;

		CImg<unsigned char> img(nx, ny, 1, 3);
		CImgDisplay canvas(img, "RayTracing Test", 0);

		Vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
		Vec3 horizontal(4.0f, 0.0f, 0.0f);
		Vec3 vertical(0.0f, 2.0f, 0.0f);
		Vec3 origin(0.0f, 0.0f, 0.0f);

		Vec3 lookfrom(3, 7, 0);
		Vec3 lookat(0, 0, -1);
		float dist_to_focus = (lookfrom - lookat).length();
		float aperture = 1.0f;

		Camera cam(lookfrom, lookat, Vec3(0, 1, 0), 20.f,
			float(nx) / float(ny), aperture, dist_to_focus);
		
		Shape* list[11];
		list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.4, 0.2, 0.5)));
		list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Metal(Vec3(1.f, 0.5f, 0.2f), 0.6f));
		list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.f));
		list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
		list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
		list[5] = new Sphere(Vec3(0.8, -0.2, 0.7), 0.3, new Lambertian(Vec3(1.0, 0.1, 0.2)));
		list[6] = new Sphere(Vec3(0.8, -0.2, -1.3), 0.3, new Lambertian(Vec3(1.0, 0.1, 0.2)));
		list[7] = new Sphere(Vec3(0, -0.3, 0.0), 0.2, new Metal(Vec3(0.1, 1.0, 0.f), 0.f));
		list[8] = new Sphere(Vec3(0, -0.3, -2.0), 0.2, new Metal(Vec3(0.1, 1.0, 0.f), 0.f));
		list[9] = new Sphere(Vec3(-0.3, -0.3, 0.0), 0.2, new Lambertian(Vec3(1.0, 0.0, 0.f)));
		list[10] = new Sphere(Vec3(-0.3, -0.3, -2.0), 0.2, new Lambertian(Vec3(1.0, 0.0, 0.f)));
		Shape* world = new Shape_List(list,11);

		RayTraceFaster(square, nx, ny, world, img, cam, canvas);

		if (world != nullptr)
		{
			delete world;
			world = nullptr;
		}

		canvas.resize(nx, ny);
		img.mirror('y');
		std::cout << "Total time taken: " << Timer::time << std::endl;
		while (!canvas.is_closed() && !canvas.is_keyESC()) {
			img.display(canvas);
			cimg::wait(20);
		}
	}
	catch (exception & e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}


