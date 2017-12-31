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
#include "Vector.h"

namespace Math
{
	Vector::Vector() : Vector(0.0f, 0.0f, 0.0f)
	{
		return;
	}

	Vector::Vector(const float x, const float y, const float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector::Vector(const Vector& vector) : Vector(vector.x, vector.y, vector.z)
	{
		return;
	}

	Vector& Vector::operator=(const Vector& vector)
	{
		this->x = vector.x;
		this->y = vector.y;
		this->z = vector.z;
		return *this;
	}

	Vector Vector::operator+(const Vector& vector) const
	{
		Vector result(x + vector.x, y + vector.y, z + vector.z);
		return result;
	}

	Vector Vector::operator-() const
	{
		Vector result(-x, -y, -z);
		return result;
	}

	Vector Vector::operator-(const Vector& vector) const
	{
		Vector result(this->x - vector.x, this->y - vector.y, this->z - vector.z);
		return result;
	}

	Vector Vector::operator*(const float scalar) const
	{
		Vector result(this->x * scalar, this->y * scalar, this->z * scalar);
		return result;
	}

	Vector Vector::operator*(const Vector& vector) const
	{
		Vector result(x * vector.x, y * vector.y, z * vector.z);
		return result;
	}

	Vector& Vector::operator*=(const float scalar)
	{
		this->x *= scalar;
		this->y *= scalar;
		this->z *= scalar;
		return *this;
	}

	Vector Vector::crossProduct(const Vector& vector) const
	{
		Vector result(*this);
		result.crossProductize(vector);
		return result;
	}

	void Vector::crossProductize(const Vector& vector)
	{
		float x = this->y * vector.z - this->z * vector.y;
		float y = this->z * vector.x - this->x * vector.z;
		float z = this->x * vector.y - this->y * vector.x;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	const float* Vector::data() const
	{
		const float* result = &(this->x);
		return result;
	}

	float Vector::dotProduct(const Vector& vector) const
	{
		float result = this->x * vector.x + this->y * vector.y + this->z * vector.z;
		return result;
	}

	float Vector::getLength() const
	{
		float result = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		return result;
	}

	void Vector::normalize()
	{
		float length = this->getLength();
		if (length > 0.0f)
		{
			this->x /= length;
			this->y /= length;
			this->z /= length;
		}
	}

	void Vector::rotate(const Vector& vector, const float degrees)
	{
		float originalLength = this->getLength();
		Vector axis(vector);
		axis.normalize();

		float rads = Algorithms::degreesToRadians(degrees);
		float t = cos(rads);
		float ux = axis.x;
		float uxt = sin(rads) * ux;
		float uy = axis.y;
		float uyt = sin(rads) * uy;
		float uz = axis.z;
		float uzt = sin(rads) * uz;

		float matrix[] = {
			t + pow(ux, 2.0f) * (1.0f - t),
			ux * uy * (1.0f - t) - uzt,
			ux * uz * (1.0f - t) + uyt,

			uy * ux * (1.0f - t) + uzt,
			t + pow(uy, 2.0f) * (1.0f - t),
			uy * uz * (1.0f - t) - uxt,

			uz * ux * (1.0f - t) - uyt,
			uz * uy * (1.0f - t) + uxt,
			t + pow(uz, 2.0f) * (1.0f - t),
		};

		float x = matrix[0] * this->x + matrix[1] * this->y + matrix[2] * this->z;
		float y = matrix[3] * this->x + matrix[4] * this->y + matrix[5] * this->z;
		float z = matrix[6] * this->x + matrix[7] * this->y + matrix[8] * this->z;
		this->x = x;
		this->y = y;
		this->z = z;
		this->normalize();
		*this *= originalLength;
	}
}
