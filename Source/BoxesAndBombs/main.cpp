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

#include <shlobj.h>
#include <windows.h>
#include "Game.h"

#ifdef _DEBUG
int main(int argc, char** argv)
#else
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	Options options;
	TCHAR appDataPath[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appDataPath)))
	{
		auto dataPath = string(appDataPath) + "\\YetiCave\\Boxes and Bombs\\";
		size_t pos = dataPath.find_first_of('\\');
		options.dataPath = dataPath;
		while ((pos = dataPath.find_first_of('\\', pos + 1)) != string::npos)
		{
			string subPath = dataPath.substr(0, pos);
			CreateDirectory(subPath.c_str(), nullptr);
		}
	}

	string filePath = "";
	HMODULE module = GetModuleHandle(NULL);
	if (module != NULL)
	{
		TCHAR moduleFilePath[MAX_PATH];
		GetModuleFileName(module, moduleFilePath, sizeof(moduleFilePath));
		filePath = string(moduleFilePath);
	}

	size_t delimiterIndex = filePath.find_last_of('\\');
	string folderPath = delimiterIndex != string::npos ? filePath.substr(0, delimiterIndex + 1) : filePath;
	options.resourcesPath = folderPath + "Resources\\";
	options.load();

	Game game(options);
	ResultCode result = game.run();
	options.save();

	return (int)result;
}
