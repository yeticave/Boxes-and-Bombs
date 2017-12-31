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

#include "Camera.h"

Camera::Camera() : eye(0.0f, 0.0f, 10.0f), up(0.0f, 1.0f, 0.0f)
{
	this->distance = 10.0f;
	this->isGimbalLocked = false;
	this->maxDistance = 20.0f;
	this->minDistance = 5.0f;
	this->pitchRotation = 0.0f;
	this->yawRotation = 0.0f;
}

void Camera::gimbalRotate()
{
	this->eye = Vector(0.0f, 0.0f, 1.0f);
	this->up = Vector(0.0f, 1.0f, 0.0f);
	this->yawEye(this->yawRotation);
	this->pitchEye(this->pitchRotation);
}

void Camera::panToBack()
{
	this->panToFront();
	this->yaw(180.0f);
}

void Camera::panToBottom()
{
	this->panToFront();
	this->pitch(-90.0f);
}

void Camera::panToFront()
{
	this->yawRotation = 0.0f;
	this->pitchRotation = 0.0f;
	this->eye = Vector(0.0f, 0.0f, 1.0f);
	this->up = Vector(0.0f, 1.0f, 0.0f);
	this->update();
}

void Camera::panToLeft()
{
	this->panToFront();
	this->yaw(-90.0f);
}

void Camera::panToRight()
{
	this->panToFront();
	this->yaw(90.0f);
}

void Camera::panToTop()
{
	this->panToFront();
	this->pitch(90.0f);
}

void Camera::pitch(const float degrees)
{
	if (this->isGimbalLocked)
	{
		const float limit = 80.0f;
		this->pitchRotation += degrees;
		if (this->pitchRotation > limit)
		{
			this->pitchRotation = limit;
		}
		else if (this->pitchRotation < -limit)
		{
			this->pitchRotation = -limit;
		}
		this->gimbalRotate();
	}
	else
	{
		this->pitchEye(degrees);
	}
}

void Camera::pitchEye(const float degrees)
{
	Vector right = this->up.crossProduct(this->eye);
	this->up.rotate(right, degrees);
	this->up.normalize();
	this->eye = right.crossProduct(this->up);
	this->update();
}

void Camera::roll(const float degrees)
{
	if (!this->isGimbalLocked)
	{
		Vector right = this->up.crossProduct(this->eye);
		right.rotate(this->eye, degrees);
		this->up = this->eye.crossProduct(right);
		this->up.normalize();
		this->update();
	}
}

void Camera::setDistance(const float distance)
{
	float newDistance = distance;
	if (newDistance > this->maxDistance)
	{
		newDistance = this->maxDistance;
	}
	else if (newDistance < this->minDistance)
	{
		newDistance = this->minDistance;
	}
	this->eye.normalize();
	this->eye *= newDistance;
	this->distance = newDistance;
}

void Camera::setGimbalLocking(const bool doEnable)
{
	this->isGimbalLocked = doEnable;
	this->panToFront();
}

void Camera::setMaxDistance(const float distance)
{
	this->maxDistance = distance;
	this->update();
}

void Camera::setMinDistance(const float distance)
{
	this->minDistance = distance;
	this->update();
}

void Camera::update()
{
	this->setDistance(this->distance);
}

void Camera::yaw(const float degrees)
{
	if (this->isGimbalLocked)
	{
		this->yawRotation += degrees;
		this->gimbalRotate();
	}
	else
	{
		this->yawEye(degrees);
	}
}

void Camera::yawEye(const float degrees)
{
	Vector right = this->up.crossProduct(this->eye);
	right.rotate(this->up, degrees);
	this->eye = right.crossProduct(up);
	this->update();
}

void Camera::zoom(const float distance)
{
	float newDistance = this->distance + distance;
	this->setDistance(newDistance);
}
