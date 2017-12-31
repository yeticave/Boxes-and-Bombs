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

#include "ResourceManager.h"
#include "Math/Algorithms.h"
#include "Field.h"

const TextureAtlas* Field::texture = nullptr;

Field::Field(ResourceManager& resourceManager) : options(resourceManager.options)
{
	if (!Field::texture)
	{
		Field::texture = resourceManager.getTextureAtlas("textures.tga");
	}
}

void Field::draw(const Camera& camera)
{
	if (this->isRevealed)
	{
		bool isSaturated = this->neighbourMarkCount == this->neighbourBombCount;
		bool canShowIndicator = !this->options.doHideSaturatedIndicators || !isSaturated || this->isBombed;
		bool hasIndicator = (this->neighbourBombCount > 0 || this->isBombed) && canShowIndicator;
		bool doShowWireFrame = this->options.wireframingMode == WireframingMode::billboards && hasIndicator
			|| this->options.wireframingMode == WireframingMode::all;

		if (doShowWireFrame)
		{
			const float black[3] = { 0.0f, 0.0f, 0.0f };
			glPolygonMode(GL_FRONT, GL_LINE);
			this->drawBox(black);
		}

		if (this->isBombed || hasIndicator)
		{
			if (this->doUpdateBillboard)
			{
				this->updateBillboard(camera);
				this->doUpdateBillboard = false;
			}

			glPolygonMode(GL_FRONT, GL_FILL);
			glBindTexture(GL_TEXTURE_2D, Field::texture->address);
			this->drawBillboard();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	else
	{
		glPolygonMode(GL_FRONT, GL_FILL);
		glBindTexture(GL_TEXTURE_2D, Field::texture->address);
		this->drawBox(this->color.data());
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Field::drawBillboard() const
{
	glBegin(GL_QUADS);
	glColor3fv(this->color.data());
	for (int i = 0; i < 4; ++i)
	{
		const Vector* vertex = &this->billboardVertices[i];
		glTexCoord2fv(this->textureCoordinates.data() + i * 2);
		glVertex3fv(vertex->data());
	}
	glEnd();
}

void Field::drawBox(const float* const color) const
{
	const int faces[6][4] = {
		{ 4, 5, 6, 7 }, // front
		{ 5, 1, 2, 6 }, // right
		{ 1, 0, 3, 2 }, // back
		{ 0, 4, 7, 3 }, // left
		{ 7, 6, 2, 3 }, // top
		{ 0, 1, 5, 4 }, // bottom
	};

	glBegin(GL_QUADS);
	glColor3fv(color);
	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			int vertex = faces[i][j];
			glTexCoord2fv(this->textureCoordinates.data() + j * 2);
			glVertex3fv(this->boxVertices[vertex].data());
		}
	}
	glEnd();
}

array<Vector, 8> Field::getVertices(const float size) const
{
	array<Vector, 8> result;
	float sizeOffset = size / 2.0f;
	float x = this->position.x;
	float y = this->position.y;
	float z = this->position.z;

	//    3-------2
	//   /|      /|
	//  / |     / |
	// 7--+----6  |
	// |  0----+--1
	// | /     | /
	// 4-------5

	// bottom left back
	result[0].x = x - sizeOffset;
	result[0].y = y - sizeOffset;
	result[0].z = z - sizeOffset;

	// bottom right back
	result[1].x = x + sizeOffset;
	result[1].y = y - sizeOffset;
	result[1].z = z - sizeOffset;

	// top right back
	result[2].x = x + sizeOffset;
	result[2].y = y + sizeOffset;
	result[2].z = z - sizeOffset;

	// top left back
	result[3].x = x - sizeOffset;
	result[3].y = y + sizeOffset;
	result[3].z = z - sizeOffset;

	// bottom left front
	result[4].x = x - sizeOffset;
	result[4].y = y - sizeOffset;
	result[4].z = z + sizeOffset;

	// bottom right front
	result[5].x = x + sizeOffset;
	result[5].y = y - sizeOffset;
	result[5].z = z + sizeOffset;

	// top right front
	result[6].x = x + sizeOffset;
	result[6].y = y + sizeOffset;
	result[6].z = z + sizeOffset;

	// top left front
	result[7].x = x - sizeOffset;
	result[7].y = y + sizeOffset;
	result[7].z = z + sizeOffset;

	return result;
}

float Field::intersect(const Ray& ray) const
{
	const int faces[12][3] = {
		// bottom left back
		{ 0, 3, 1 },
		{ 0, 4, 3 },
		{ 0, 1, 4 },

		// top right back
		{ 2, 1, 3 },
		{ 2, 6, 1 },
		{ 2, 3, 6 },

		// bottom right front
		{ 5, 6, 4 },
		{ 5, 1, 6 },
		{ 5, 4, 1 },

		// top left front
		{ 7, 4, 6 },
		{ 7, 3, 4 },
		{ 7, 6, 3 },
	};
	float result = -1.0f;

	for (int i = 0; i < 12; ++i)
	{
		Vector a = this->boxVertices[faces[i][0]];
		Vector b = this->boxVertices[faces[i][1]];
		Vector c = this->boxVertices[faces[i][2]];
		Triangle triangle(a, b, c);
		float distance = Algorithms::intersection(ray, triangle);
		if (distance > 0.0f)
		{
			if (result < 0.0f || distance < result)
			{
				result = distance;
			}
		}
	}

	return result;
}

bool Field::mark()
{
	this->isMarked = !this->isMarked;
	return this->isMarked;
	this->updateModel();
}

void Field::refresh()
{
	if (this->isRevealed)
	{
		this->doUpdateBillboard = true;
	}
	this->updateModel();
}

void Field::reset()
{
	this->doUpdateBillboard = false;
	this->isBombed = false;
	this->isMarked = false;
	this->isProtected = false;
	this->isRevealed = false;
	this->isSelected = false;
	this->neighbourBombCount = 0;
	this->neighbourMarkCount = 0;
	this->updateModel();
}

void Field::reveal()
{
	this->doUpdateBillboard = true;
	this->isRevealed = true;
	this->updateModel();
}

void Field::select(const bool& doSelect)
{
	this->isSelected = doSelect;
	this->updateModel();
}

void Field::setPosition(const float x, const float y, const float z)
{
	this->position = Vector(x, y, z);
	this->updateModel();
}

void Field::setSize(const float size)
{
	this->size = size;
}

void Field::updateBillboard(const Camera& camera)
{
	Vector eye(camera.eye);
	Vector up(camera.up);
	Vector left = eye.crossProduct(up);
	float scale = this->isBombed ? this->size / 4.0f : this->size / 2.0f;

	eye.normalize();
	up.normalize();
	left.normalize();
	up *= scale;
	left *= scale;

	Vector down = up * -1.0f;
	Vector right = left * -1.0f;
	this->billboardVertices = {
		down + left + this->position,
		down + right + this->position,
		up + right + this->position,
		up + left + this->position,
	};
}

void Field::updateModel()
{
	const array<float, 3> blue = { 0.25f, 0.25f, 1.0f };
	const array<float, 3> green = { 0.0f, 0.5f, 0.0f };
	const array<float, 3> orange = { 1.0f, 0.5f, 0.0f };
	const array<float, 3> red = { 0.75f, 0.0f, 0.0f };
	const array<float, 3> violet = { 0.5f, 0.0f, 1.0f };
	const array<float, 3> white = { 1.0f, 1.0f, 1.0f };
	const array<float, 3> yellow = { 1.0f, 1.0f, 0.0f };

	if (!this->isRevealed)
	{
		bool doUseColors = this->options.colorblindMode == ColorblindMode::colors
			|| this->options.colorblindMode == ColorblindMode::both;
		bool doUseSymbols = this->options.colorblindMode == ColorblindMode::symbols
			|| this->options.colorblindMode == ColorblindMode::both;

		array<float, 3> color = white;
		if (doUseColors)
		{
			if (this->isMarked)
			{
				color = this->isSelected ? orange : red;
			}
			else if (this->isSelected)
			{
				color = green;
			}
		}

		int textureIndex = 0;
		if (doUseSymbols)
		{
			if (this->isMarked)
			{
				textureIndex = this->isSelected ? 3 : 2;
			}
			else if (this->isSelected)
			{
				textureIndex = 1;
			}
		}

		float size = this->isMarked && this->options.doShrinkMarkedBoxes ? this->size / 2.0f : this->size;
		this->boxVertices = this->getVertices(size);
		this->color = color;
		this->textureCoordinates = Field::texture->getTextureCoordinates(16, textureIndex, 0, true);
	}
	else if (!this->isBombed)
	{
		this->boxVertices = this->getVertices(this->size);
		this->textureCoordinates = Field::texture->getTextureCoordinates(16, this->neighbourBombCount + 3, 0, true);
		switch (this->neighbourBombCount)
		{
			case 1:
				this->color = violet;
				break;

			case 2:
				this->color = blue;
				break;

			case 3:
				this->color = green;
				break;

			case 4:
				this->color = yellow;
				break;

			case 5:
				this->color = orange;
				break;

			case 6:
				this->color = red;
				break;
		}
	}
	else
	{
		this->boxVertices = this->getVertices(this->size);
		this->textureCoordinates = Field::texture->getTextureCoordinates(16, 10, 0, false);
		this->color = white;
	}
}
