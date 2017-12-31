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

#ifndef IS_VECTOR_INCLUDED
#define IS_VECTOR_INCLUDED

namespace Math
{
	class Vector
	{
		public:
		float x;
		float y;
		float z;

		Vector();
		Vector(const float x, const float y, const float z);
		Vector(const Vector& vector);

		Vector& operator=(const Vector& vector);
		Vector operator+(const Vector& vector) const;
		Vector operator-() const;
		Vector operator-(const Vector& vector) const;
		Vector operator*(const float scalar) const;
		Vector operator*(const Vector& vector) const;
		Vector& operator*=(const float scalar);

		Vector crossProduct(const Vector& vector) const;
		void crossProductize(const Vector& vector);
		const float* data() const;
		float dotProduct(const Vector& vector) const;
		float getLength() const;
		void normalize();
		void rotate(const Vector& vector, const float degrees);
	};
}

#endif
