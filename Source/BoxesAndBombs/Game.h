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

#ifndef IS_GAME_INCLUDED
#define IS_GAME_INCLUDED

#include "Camera.h"
#include "Difficulty.h"
#include "GameStatus.h"
#include "Logger.h"
#include "ResourceManager.h"
#include "ResultCode.h"
#include "Scene.h"
#include "UserInterface.h"
#include "Configuration/Options.h"
using namespace Configuration;

class Game
{
	public:
	bool didWin;
	Difficulty difficulty;
	bool isNewRecord;
	Logger logger;
	float mouseX;
	float mouseY;
	Options& options;
	ResourceManager resourceManager;
	Scene scene;
	GameStatus status;
	double time;
	UserInterface userInterface;
	int viewportWidth;
	int viewportHeight;
	Game(Options& options);
	void finishGame(const bool didWin);
	float getAspectRatio() const;
	void newGame();
	void panToPerspective();
	void refreshMousePosition();
	ResultCode run();
	void setDifficulty(const Difficulty difficulty);
	void shutdown();
	void startGame();
	int toggle(const int value, const int step, const int min, const int max, const bool doIncrease) const;
	void toggleCameraMode(const bool doIncrease);
	void toggleColorblindMode(const bool doIncrease);
	void toggleDragButton(const bool doIncrease);
	void toggleDragSensitivity(const bool doIncrease);
	void toggleDragThreshold(const bool doIncrease);
	void toggleMarkingMode(const bool doIncrease);
	void toggleSaturatedIndicatorDisplay(const bool doIncrease);
	void toggleSoundVolume(const bool doIncrease);
	void toggleWireframingMode(const bool doIncrease);

	private:
	Camera camera;
	int defeatSound;
	static Game* instance;
	bool isRunning;
	int victorySound;
	GLFWwindow* window;

	static void handleErrorEvent(int error, const char* description);
	void handleKeyPressEvent(const int key, const int scancode, const int action, const int mods);
	static void handleKeyPressEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	void handleMouseClickEvent(const int button, const int action);
	static void handleMouseClickEvent(GLFWwindow* window, int button, int action, int mods);
	void handleMouseMovementEvent(const float x, const float y);
	static void handleMouseMovementEvent(GLFWwindow* window, double xpos, double ypos);
	void handleMouseScrollEvent(const float delta);
	static void handleMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset);
	void handleWindowResizeEvent(const int width, const int height);
	static void handleWindowResizeEvent(GLFWwindow* window, int width, int height);
};

#endif
