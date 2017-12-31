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

#include "Label.h"

namespace Gui
{
	const TextureAtlas* Label::texture = nullptr;
	char Label::fontSizing[103];

	Label::Label(ResourceManager& resourceManager)
	{
		if (!Label::texture)
		{
			Label::texture = resourceManager.getTextureAtlas("textures.tga");
			ifstream file;

			file.open((resourceManager.options.resourcesPath + "font.bin").c_str(), ios::binary);
			if (file.is_open())
			{
				for (int i = 0; i < 103; ++i)
				{
					char width;
					file.rdbuf()->sgetn((char*)&width, sizeof(char));
					Label::fontSizing[i] = width;
				}
			}
			file.close();
		}

		this->color = { 1.0f, 0.0f, 0.0f };
		this->isVisible = true;
		this->scale = 128.0f;
		this->textureCoordinates = nullptr;
		this->vertices = nullptr;
	}

	void Label::draw() const
	{
		this->draw(this->color);
	}

	void Label::draw(const array<float, 3>& color) const
	{
		if (this->vertices)
		{
			glBindTexture(GL_TEXTURE_2D, Label::texture->address);
			glBegin(GL_QUADS);
			for (int i = 0; i < this->length; ++i)
			{
				glColor3fv(color.data());
				glTexCoord2fv(&this->textureCoordinates[i * 8]);
				glVertex2fv(&this->vertices[i * 8]);
				glTexCoord2fv(&this->textureCoordinates[i * 8 + 2]);
				glVertex2fv(&this->vertices[i * 8 + 2]);
				glTexCoord2fv(&this->textureCoordinates[i * 8 + 4]);
				glVertex2fv(&this->vertices[i * 8 + 4]);
				glTexCoord2fv(&this->textureCoordinates[i * 8 + 6]);
				glVertex2fv(&this->vertices[i * 8 + 6]);
			}
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	bool Label::isDrawable(char character) const
	{
		bool result = character < 127 && character > 32;
		return result;
	}

	void Label::setPosition(const float x, const float y, const Pivot pivot)
	{
		this->x = x;
		this->y = y;
		this->pivot = pivot;
		this->refresh();
	}

	void Label::refresh()
	{
		int characterCount = this->text.length();
		const char* characters = this->text.c_str();

		this->length = 0;
		for (int i = 0; i < characterCount; ++i)
		{
			if (this->isDrawable(characters[i]))
			{
				++this->length;
			}
		}

		if (this->textureCoordinates)
		{
			delete[] this->textureCoordinates;
		}
		if (this->vertices)
		{
			delete[] this->vertices;
		}

		int drawableIndex = 0;
		float pixelSize = 1.0f / 16.0f;
		float characterHeight = this->scale * pixelSize * 13.0f;
		float spacing = this->scale * pixelSize;
		float rowOffset = 0.0f;
		float rowWidth = 0.0f;

		this->width = 0.0f;
		this->height = characterHeight;
		this->textureCoordinates = new float[this->length * 8];
		this->vertices = new float[this->length * 8];

		for (int i = 0; i < characterCount; ++i)
		{
			char character = characters[i];

			if (this->isDrawable(character))
			{
				if (rowWidth > 0.0f)
				{
					rowWidth -= spacing * 2.5f;
				}

				this->vertices[drawableIndex * 8] = rowWidth;
				this->vertices[drawableIndex * 8 + 1] = this->scale + rowOffset;
				this->vertices[drawableIndex * 8 + 2] = rowWidth + this->scale;
				this->vertices[drawableIndex * 8 + 3] = this->scale + rowOffset;
				this->vertices[drawableIndex * 8 + 4] = rowWidth + this->scale;
				this->vertices[drawableIndex * 8 + 5] = rowOffset;
				this->vertices[drawableIndex * 8 + 6] = rowWidth;
				this->vertices[drawableIndex * 8 + 7] = rowOffset;

				int atlasIndex = character - 33;
				int atlasX = atlasIndex % 16;
				int atlasY = (atlasIndex - atlasX) / 16 + 1;
				array<float, 8> textureCoordinates = Label::texture->getTextureCoordinates(16, atlasX, atlasY, true);
				for (int j = 0; j < 8; ++j)
				{
					this->textureCoordinates[drawableIndex * 8 + j] = textureCoordinates[j];
				}

				auto characterWidth = (float)Label::fontSizing[atlasIndex];
				rowWidth += characterWidth * this->scale * pixelSize;
				if (rowWidth > this->width)
				{
					this->width = rowWidth;
				}
				++drawableIndex;
			}
			else if (character == 32)
			{
				rowWidth += spacing * 2.0f;
			}
			else if (character == 10)
			{
				rowOffset += characterHeight + spacing;
				rowWidth = 0.0f;
			}
		}
		this->height += rowOffset;

		float xOffset = 0.0f;
		float yOffset = 0.0f;

		switch (this->pivot)
		{
			case Pivot::bottom:
				xOffset = this->width / -2.0f;
				yOffset = -this->height;
				break;

			case Pivot::bottomLeft:
				yOffset = -this->height;
				break;

			case Pivot::bottomRight:
				xOffset = -this->width;
				yOffset = -this->height;
				break;

			case Pivot::center:
				xOffset = this->width / -2.0f;
				yOffset = this->height / -2.0f;
				break;

			case Pivot::left:
				yOffset = this->height / -2.0f;
				break;

			case Pivot::right:
				xOffset = -this->width;
				yOffset = this->height / -2.0f;
				break;

			case Pivot::top:
				xOffset = this->width / -2.0f;
				break;

			case Pivot::topRight:
				xOffset = -this->width;
				break;
		}

		this->boundaries = {
			this->x + xOffset,
			this->y + yOffset,
			this->x + this->width + xOffset,
			this->y + this->height + yOffset,
		};

		for (int i = 0; i < this->length * 8; i += 2)
		{
			this->vertices[i] += this->x + xOffset;
			this->vertices[i + 1] += this->y + yOffset;
		}
	}

	void Label::setScale(const float scale)
	{
		this->scale = scale;
		this->refresh();
	}

	void Label::setText(const string& text)
	{
		if (text != this->text)
		{
			this->text = text;
			this->refresh();
		}
	}
}
