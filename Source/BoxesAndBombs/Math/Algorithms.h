/*
Boxes and Bombs
Version 1.0.0, December 31st, 2017

Copyright (C) 2017 Gabor Soos

This software is provided 'as-is', without any express or implied warranty.In no
event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim
that you wrote the original software. If you use this software in a product,
an acknowledgment in the product documentation would be appreciated but is
not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef IS_ALGORITHMS_INCLUDED
#define IS_ALGORITHMS_INCLUDED

#include "../main.h"
#include "PerspectiveFrustrum.h"
#include "Ray.h"
#include "Triangle.h"
#include "Vector.h"
using namespace std;

namespace Math
{
	class Algorithms
	{
		public:
		const static float INTERSECTION_THRESHOLD;
		const static double PI;
		static array<float, 16> cameraMatrix(const Vector& eye, const Vector& focus, const Vector& up);
		static float cubeSpaceDiagonal(const float size);
		static float degreesToRadians(const float degrees);
		static float intersection(const Ray& ray, const Triangle& triangle);
		static PerspectiveFrustrum perspectiveFrustrum(const GLdouble fov, const GLdouble aspectRatio,
			const GLdouble zNear, const GLdouble zFar);
		static Vector worldPosition(const float x, const float y, const float z);
		static Vector worldPosition(const float x, const float y);
	};
}

#endif
