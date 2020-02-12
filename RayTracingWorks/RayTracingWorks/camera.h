#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Vec3.h"
#include "Ray.h"
//==================================================================================================
// Written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is distributed
// without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication along
// with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==================================================================================================


class Camera {
public:
	Camera() {
		lower_left_corner = Vec3(-2.0, -1.0, -1.0);
		horizontal = Vec3(4.0, 0.0, 0.0);
		vertical = Vec3(0.0, 2.0, 0.0);
		origin = Vec3(0.0, 0.0, 0.0);
	}

	Ray get_ray(float u, float v) {
		return Ray(origin,
			lower_left_corner + u * horizontal + v * vertical - origin);
	}

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif