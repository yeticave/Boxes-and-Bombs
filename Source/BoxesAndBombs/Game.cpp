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

#include "Game.h"
using namespace std;

Game* Game::instance = nullptr;

Game::Game(Options& options) : options(options), resourceManager(options), scene(*this, this->camera),
userInterface(*this)
{
	Game::instance = this;
	this->camera.setGimbalLocking(this->options.doGimbalLockCamera);
	this->defeatSound = this->resourceManager.getSound("defeat");
	this->victorySound = this->resourceManager.getSound("victory");
	this->isRunning = true;
}

void Game::finishGame(bool didWin)
{
	double record = this->options.records[(int)this->difficulty];
	this->status = GameStatus::finished;
	this->didWin = didWin;

	this->resourceManager.playSound(didWin ? this->victorySound : this->defeatSound);
	if (didWin && (this->time < record || record < 0.0))
	{
		this->options.records[(int)this->difficulty] = this->time;
		this->isNewRecord = true;
		this->options.save();
	}
	else
	{
		this->isNewRecord = false;
	}

	this->userInterface.showPlayView();
}

float Game::getAspectRatio() const
{
	float result = (float)this->viewportWidth / (float)this->viewportHeight;
	return result;
}

void Game::handleErrorEvent(int error, const char* description)
{
	Game::instance->logger.logError(string(description));
}

void Game::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods)
{
	if (!this->userInterface.handleKeyPressEvent(key, scancode, action, mods))
	{
		this->scene.handleKeyPressEvent(key, scancode, action, mods);
	}
}

void Game::handleKeyPressEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Game::instance->handleKeyPressEvent(key, scancode, action, mods);
}

void Game::handleMouseClickEvent(const int button, const int action)
{
	if (!this->userInterface.handleMouseClickEvent(button, action))
	{
		this->scene.handleMouseClickEvent(button, action);
	}
}

void Game::handleMouseClickEvent(GLFWwindow* window, int button, int action, int mods)
{
	Game::instance->handleMouseClickEvent(button, action);
}

void Game::handleMouseMovementEvent(const float x, const float y)
{
	this->mouseX = x;
	this->mouseY = y;
	if (!this->userInterface.handleMouseMovementEvent(x, y))
	{
		this->scene.handleMouseMovementEvent(x, y);
	}
}

void Game::handleMouseMovementEvent(GLFWwindow* window, double xpos, double ypos)
{
	Game::instance->handleMouseMovementEvent((float)xpos, (float)ypos);
}

void Game::handleMouseScrollEvent(const float delta)
{
	if (!this->userInterface.handleMouseScrollEvent(delta))
	{
		this->scene.handleMouseScrollEvent(delta);
	}
}

void Game::handleMouseScrollEvent(GLFWwindow* window, double xoffset, double yoffset)
{
	Game::instance->handleMouseScrollEvent((float)yoffset);
}

void Game::handleWindowResizeEvent(const int width, const int height)
{
	this->viewportWidth = width;
	this->viewportHeight = height;
	glViewport(0, 0, this->viewportWidth, this->viewportHeight);
	this->userInterface.refresh();
}

void Game::handleWindowResizeEvent(GLFWwindow* window, int width, int height)
{
	Game::instance->handleWindowResizeEvent(width, height);
}

void Game::newGame()
{
	this->status = GameStatus::ready;
	this->time = 0.0;
	this->scene.reset();
}

void Game::panToPerspective()
{
	this->camera.panToFront();
	this->camera.yaw(30.0f);
	this->camera.pitch(-30.0f);
	this->scene.refresh();
}

void Game::refreshMousePosition()
{
	this->handleMouseMovementEvent(this->mouseX, this->mouseY);
}

ResultCode Game::run()
{
	auto result = ResultCode::success;

	if (!glfwInit())
	{
		result = ResultCode::glfwInitializationFailed;
	}

	glfwSetErrorCallback(Game::handleErrorEvent);
	this->window = glfwCreateWindow(640, 640, "Boxes and Bombs", nullptr, nullptr);

	if (!this->window)
	{
		this->isRunning = false;
		result = ResultCode::windowCreationFailed;
	}

	glfwMakeContextCurrent(this->window);
	glfwGetFramebufferSize(this->window, &this->viewportWidth, &this->viewportHeight);
	glViewport(0, 0, this->viewportWidth, this->viewportHeight);
	glfwSetCursorPosCallback(this->window, Game::handleMouseMovementEvent);
	glfwSetKeyCallback(this->window, Game::handleKeyPressEvent);
	glfwSetMouseButtonCallback(this->window, Game::handleMouseClickEvent);
	glfwSetScrollCallback(this->window, Game::handleMouseScrollEvent);
	glfwSetWindowSizeCallback(this->window, Game::handleWindowResizeEvent);

	glAlphaFunc(GL_GEQUAL, 0.1f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.87f, 0.75f, 0.5f, 0.0f);
	glClearDepth(1);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0, 1);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	double lastTime = glfwGetTime();
	this->resourceManager.initialize();
	this->scene.refresh();
	this->userInterface.refresh();
	this->setDifficulty(Difficulty::simple);
	this->newGame();
	this->status = GameStatus::uninitialized;

	while (this->isRunning)
	{
		this->scene.draw();
		this->userInterface.draw();
		glfwSwapBuffers(this->window);

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		glfwPollEvents();
		if (this->status == GameStatus::started)
		{
			this->time += deltaTime;
		}
		this->scene.update(deltaTime);
		this->userInterface.update(deltaTime);

		if (glfwWindowShouldClose(this->window))
		{
			this->shutdown();
		}
	}

	if (this->window)
	{
		glfwDestroyWindow(this->window);
	}

	glfwTerminate();
	this->logger.logDebug(to_string((int)result));

	return result;
}

void Game::setDifficulty(Difficulty difficulty)
{
	this->difficulty = difficulty;
	switch (difficulty)
	{
		case Difficulty::simple:
			this->scene.setSize(4, 4, 4, 16);
			break;

		case Difficulty::medium:
			this->scene.setSize(6, 6, 6, 48);
			break;

		case Difficulty::complex:
			this->scene.setSize(8, 8, 8, 128);
			break;
	}

	this->newGame();
}

void Game::shutdown()
{
	this->isRunning = false;
	glfwSetWindowShouldClose(this->window, GL_TRUE);
}

void Game::startGame()
{
	this->status = GameStatus::started;
}

int Game::toggle(const int value, const int step, const int min, const int max, const bool doIncrease) const
{
	int delta = doIncrease ? step : -step;
	int result = value + delta;

	if (result > max)
	{
		result = min;
	}
	else if (result < min)
	{
		result = max;
	}

	return result;
}

void Game::toggleCameraMode(bool doIncrease)
{
	this->options.doGimbalLockCamera = !this->options.doGimbalLockCamera;
	this->camera.setGimbalLocking(this->options.doGimbalLockCamera);
	this->panToPerspective();
	this->scene.refresh();
	this->userInterface.refresh();
}

void Game::toggleColorblindMode(bool doIncrease)
{
	int newValue = this->toggle((int)this->options.colorblindMode, 1, 0, (int)ColorblindMode::symbols, doIncrease);
	this->options.colorblindMode = (ColorblindMode)newValue;
	this->scene.refresh();
	this->userInterface.refresh();
}

void Game::toggleDragButton(bool doIncrease)
{
	int newValue = this->toggle((int)this->options.dragButton, 1, 0, (int)DragButton::none, doIncrease);
	this->options.dragButton = (DragButton)newValue;
	this->userInterface.refresh();
}

void Game::toggleDragSensitivity(bool doIncrease)
{
	int newValue = this->toggle(this->options.dragSensitivity, 1, 1, 10, doIncrease);
	this->options.dragSensitivity = newValue;
	this->userInterface.refresh();
}

void Game::toggleDragThreshold(bool doIncrease)
{
	int newValue = this->toggle(this->options.dragThreshold, 5, 5, 50, doIncrease);
	this->options.dragThreshold = newValue;
	this->userInterface.refresh();
}

void Game::toggleMarkingMode(bool doIncrease)
{
	this->options.doShrinkMarkedBoxes = !this->options.doShrinkMarkedBoxes;
	this->scene.refresh();
	this->userInterface.refresh();
}

void Game::toggleSaturatedIndicatorDisplay(bool doIncrease)
{
	this->options.doHideSaturatedIndicators = !this->options.doHideSaturatedIndicators;
	this->scene.refresh();
	this->userInterface.refresh();
}

void Game::toggleSoundVolume(const bool doIncrease)
{
	int newValue = this->toggle(this->options.soundVolume, 5, 0, 100, doIncrease);
	this->options.soundVolume = newValue;
	this->userInterface.refresh();
}

void Game::toggleWireframingMode(bool doIncrease)
{
	int newValue = this->toggle((int)this->options.wireframingMode, 1, 0, (int)WireframingMode::all, doIncrease);
	this->options.wireframingMode = (WireframingMode)newValue;
	this->scene.refresh();
	this->userInterface.refresh();
}
