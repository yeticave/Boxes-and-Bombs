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

#include "Algorithms.h"

namespace Math
{
	const double Algorithms::PI = 3.1415926535897932384626433832795;

	array<float, 16> Algorithms::cameraMatrix(const Vector& eye, const Vector& focus, const Vector& up)
	{
		Vector vN = eye - focus;
		Vector vU = up.crossProduct(vN);
		vU.normalize();
		vN.normalize();

		Vector vV = vN.crossProduct(vU);

		array<float, 16> result = {
			vU.x, vV.x, vN.x, 0.0f,
			vU.y, vV.y, vN.y, 0.0f,
			vU.z, vV.z, vN.z, 0.0f,
			-eye.dotProduct(vU), -eye.dotProduct(vV), -eye.dotProduct(vN), 1.0f
		};

		return result;
	}

	float Algorithms::cubeSpaceDiagonal(const float size)
	{
		float result = sqrt(3 * size * size);
		return result;
	}

	float Algorithms::degreesToRadians(const float degrees)
	{
		float result = (float)((double)degrees * Algorithms::PI / 180.0);
		return result;
	}

	float Algorithms::intersection(const Ray& ray, const Triangle& triangle)
	{
		// Möller–Trumbore intersection algorithm
		float threshold = 0.0000001f;
		Vector edgeAb = triangle.b - triangle.a;
		Vector edgeAc = triangle.c - triangle.a;
		Vector h = ray.direction.crossProduct(edgeAc);
		float a = edgeAb.dotProduct(h);
		float result = -1.0f;

		if (a > -threshold && a < threshold)
		{
			return result;
		}

		float f = 1.0f / a;
		Vector s = ray.origin - triangle.a;
		float u = f * (s.dotProduct(h));
		if (u < 0.0 || u > 1.0)
		{
			return result;
		}

		Vector q = s.crossProduct(edgeAb);
		float v = f * ray.direction.dotProduct(q);
		if (v < 0.0f || u + v > 1.0f)
		{
			return result;
		}

		float t = f * edgeAc.dotProduct(q);
		if (t > threshold)
		{
			result = t;
		}

		return result;
	}

	PerspectiveFrustrum Algorithms::perspectiveFrustrum(const GLdouble fov, const GLdouble aspectRatio,
		const GLdouble zNear, const GLdouble zFar)
	{
		PerspectiveFrustrum result;
		result.height = tan(Algorithms::degreesToRadians((float)fov / 2.0f)) * zNear;
		result.width = result.height * aspectRatio;
		return result;
	}

	Vector Algorithms::worldPosition(const float x, const float y, const float z)
	{
		double modelview[16];
		double posX;
		double posY;
		double posZ;
		double projection[16];
		int viewport[4];
		float winX = (float)x;
		float winY;
		float winZ;

		glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
		glGetDoublev(GL_PROJECTION_MATRIX, projection);
		glGetIntegerv(GL_VIEWPORT, viewport);
		winY = (float)viewport[3] - (float)y;
		if (z >= 0.0)
		{
			winZ = (float)z;
		}
		else
		{
			glReadPixels((int)winX, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
		}
		gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		Vector result((float)posX, (float)posY, (float)posZ);
		return result;
	}

	Vector Algorithms::worldPosition(const float x, const float y)
	{
		return Algorithms::worldPosition(x, y, -1.0);
	}
}
