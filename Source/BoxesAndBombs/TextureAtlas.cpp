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

#include "TextureAtlas.h"

TextureAtlas::TextureAtlas()
{
	this->width = 16;
	this->height = 16;
}

array<float, 8> TextureAtlas::getTextureCoordinates(const int size, const int x, const int y,
	const bool doHalfPixelCorrection) const
{
	// (6,7)-(4,5)
	// |         |
	// |         |
	// |         |
	// (0,1)-(2,3)
	float halfPixelSize = doHalfPixelCorrection ? 0.5f / (float)this->width : 0.0f;
	float textureSize = 1.0f / (float)size;
	array<float, 8> result = {
		x * textureSize + halfPixelSize, 1.0f - (y + 1) * textureSize + halfPixelSize,
		(x + 1) * textureSize - halfPixelSize, 1.0f - (y + 1) * textureSize + halfPixelSize,
		(x + 1) * textureSize - halfPixelSize, 1.0f - y * textureSize - halfPixelSize,
		x * textureSize + halfPixelSize, 1.0f - y * textureSize - halfPixelSize,
	};

	return result;
}
