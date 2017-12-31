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

#ifndef IS_RESOURCEMANAGER_INCLUDED
#define IS_RESOURCEMANAGER_INCLUDED

#include "main.h"
#include "TextureAtlas.h"
#include "Configuration/Options.h"
using namespace std;
using namespace SoLoud;
using namespace Configuration;

class ResourceManager
{
	public:
	const Options& options;
	ResourceManager(const Options& options);
	~ResourceManager();
	void cleanUp();
	int getSound(const string& filePath);
	const TextureAtlas* const getTextureAtlas(const string& filePath);
	void playSound(const int id);
	void initialize();
	void reloadTextures();

	private:
	bool isInitialized;
	int findSound(const string& filePath) const;
	int findTexture(const string& filePath) const;
	void loadTga(TextureAtlas* texture) const;
	Soloud soLoud;
	vector<string> soundFilePaths;
	vector<Wav*> sounds;
	vector<TextureAtlas*> textures;
};

#endif
