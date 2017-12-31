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

ResourceManager::ResourceManager(const Options& options) : options(options)
{
	this->isInitialized = false;
	this->soLoud.init();
}

ResourceManager::~ResourceManager()
{
	int textureCount = this->textures.size();
	for (int i = 0; i < textureCount; ++i)
	{
		delete this->textures[i];
	}

	this->soLoud.deinit();
}

int ResourceManager::findSound(const string& filePath) const
{
	int soundCount = this->sounds.size();
	int result = -1;

	for (int i = 0; i < soundCount; ++i)
	{
		if (this->soundFilePaths[i] == filePath)
		{
			result = i;
			break;
		}
	}

	return result;
}

int ResourceManager::findTexture(const string& filePath) const
{
	int textureCount = this->textures.size();
	int result = -1;

	for (int i = 0; i < textureCount; ++i)
	{
		if (this->textures[i]->filePath == filePath)
		{
			result = i;
			break;
		}
	}

	return result;
}

int ResourceManager::getSound(const string& filePath)
{
	int result = this->findSound(filePath);

	if (result < 0)
	{
		auto wav = new Wav();
		string fullPath = this->options.resourcesPath + filePath + ".ogg";
		wav->load(fullPath.c_str());
		this->soundFilePaths.push_back(filePath);
		this->sounds.push_back(wav);
		result = this->sounds.size() - 1;
	}

	return result;
}

const TextureAtlas* const ResourceManager::getTextureAtlas(const string& filePath)
{
	int index = this->findTexture(filePath);
	TextureAtlas* result = nullptr;

	if (index < 0)
	{
		result = new TextureAtlas();
		this->textures.push_back(result);
		result->filePath = this->options.resourcesPath + filePath;
		if (this->isInitialized)
		{
			this->loadTga(result);
		}
	}
	else
	{
		result = this->textures[index];
	}

	return result;
}

void ResourceManager::initialize()
{
	if (!this->isInitialized)
	{
		this->isInitialized = true;
		this->reloadTextures();
	}
}

void ResourceManager::loadTga(TextureAtlas* texture) const
{
	FILE* file;

	if (fopen_s(&file, texture->filePath.c_str(), "rb") == 0 && file)
	{
		unsigned char info[6];
		unsigned char type[4];
		fread(&type, sizeof(char), 3, file);
		fseek(file, 12, SEEK_SET);
		fread(&info, sizeof(char), 6, file);

		// 2 color, 3 greyscale
		if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
		{
			return;
		}

		unsigned char  bpp = info[4] / 8;
		int height = info[2] + info[3] * 256;
		int width = info[0] + info[1] * 256;
		long int size = width * height * bpp;

		texture->height = height;
		texture->width = width;
		if (bpp != 3 && bpp != 4)
		{
			return;
		}

		auto data = new unsigned char[size];
		fread(data, sizeof(unsigned char), size, file);
		for (long int i = 0; i < size; i += 4)
		{
			unsigned char blue = data[i];
			unsigned char red = data[i + 2];
			data[i] = red;
			data[i + 2] = blue;
		}

		glGenTextures(1, &texture->address);
		glBindTexture(GL_TEXTURE_2D, texture->address);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		delete[] data;
	}

	if (file)
	{
		fclose(file);
	}
}

void ResourceManager::playSound(const int id)
{
	float volume = (float)this->options.soundVolume / 100.0f;
	if (volume > 0.0f)
	{
		Wav* sound = this->sounds[id];
		this->soLoud.play(*sound, volume);
	}
}

void ResourceManager::reloadTextures()
{
	if (this->isInitialized)
	{
		int textureCount = this->textures.size();
		for (int i = 0; i < textureCount; ++i)
		{
			TextureAtlas* texture = this->textures[i];
			this->loadTga(texture);
		}
	}
}
