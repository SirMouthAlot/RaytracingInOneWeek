#ifndef __RAY_H__
#define __RAY_H__

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

#include "utils.h"


class Ray
{
public:
	Ray() {}
	Ray(const Vec3& a, const Vec3& b) { A = a; B = b; }
	Vec3 origin() const { return A; }
	Vec3 direction() const { return B; }
	Vec3 point_at_parameter(float t) const { return A + t * B; }

	Vec3 A;
	Vec3 B;
};

#endif
