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

#ifndef IS_CAMERA_INCLUDED
#define IS_CAMERA_INCLUDED

#include "Math/Vector.h"
using namespace Math;

class Camera
{
	public:
	Vector eye;
	Vector focus;
	float maxDistance;
	float minDistance;
	Vector up;

	Camera();
	void panToBack();
	void panToBottom();
	void panToFront();
	void panToLeft();
	void panToRight();
	void panToTop();
	void pitch(const float degrees);
	void roll(const float degrees);
	void setDistance(const float distance);
	void setGimbalLocking(const bool doEnable);
	void setMaxDistance(const float distance);
	void setMinDistance(const float distance);
	void update();
	void yaw(const float degrees);
	void zoom(const float distance);

	private:
	float distance;
	bool isGimbalLocked;
	float pitchRotation;
	float yawRotation;

	void gimbalRotate();
	void pitchEye(const float degrees);
	void yawEye(const float degrees);
};

#endif
