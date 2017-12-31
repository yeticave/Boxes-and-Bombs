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

#ifndef IS_FIELD_INCLUDED
#define IS_FIELD_INCLUDED

#include "main.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Configuration/Options.h"
#include "Math/Ray.h"
using namespace std;
using namespace Configuration;
using namespace Math;

class Field
{
	public:
	bool doUpdateBillboard;
	bool isBombed;
	bool isMarked;
	bool isProtected;
	bool isRevealed;
	int neighbourBombCount;
	int neighbourMarkCount;

	Field(ResourceManager& resourceManager);
	void draw(const Camera& camera);
	float intersect(const Ray& ray) const;
	bool mark();
	void refresh();
	void reset();
	void reveal();
	void select(const bool& doSelect);
	void setPosition(const float x, const float y, const float z);
	void setSize(const float size);

	private:
	array<Vector, 4> billboardVertices;
	array<Vector, 8> boxVertices;
	array<float, 3> color;
	bool isSelected;
	const Options& options;
	Vector position;
	float size;
	static const TextureAtlas* texture;
	array<float, 8> textureCoordinates;

	void drawBillboard() const;
	void drawBox(const float* const color) const;
	array<Vector, 8> getVertices(const float size) const;
	void updateBillboard(const Camera& camera);
	void updateModel();
};

#endif
