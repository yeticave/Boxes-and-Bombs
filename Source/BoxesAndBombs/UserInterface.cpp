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
#include "GameStatus.h"
#include "UserInterface.h"

UserInterface::UserInterface(Game& game) : game(game), difficultyMenuView(game), mainMenuView(game),
optionsMenuView(game), playView(game), recordsView(game)
{
	this->currentView = &this->mainMenuView;
}

void UserInterface::draw() const
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, this->game.viewportWidth, this->game.viewportHeight, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT, GL_FILL);
	this->currentView->draw();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

bool UserInterface::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods)
{
	bool result = this->currentView->handleKeyPressEvent(key, scancode, action, mods);
	return result;
}

bool UserInterface::handleMouseClickEvent(const int button, const int action)
{
	bool result = this->currentView->handleMouseClickEvent(button, action);
	return result;
}

bool UserInterface::handleMouseMovementEvent(const float x, const float y)
{
	bool result = this->currentView->handleMouseMovementEvent(x, y);
	return result;
}

bool UserInterface::handleMouseScrollEvent(const float delta)
{
	bool result = this->currentView->handleMouseScrollEvent(delta);
	return result;
}

void UserInterface::refresh()
{
	this->currentView->refresh();
}

void UserInterface::showDifficultyMenuView()
{
	this->swapView(&this->difficultyMenuView);
}

void UserInterface::showMainMenuView()
{
	this->swapView(&this->mainMenuView);
}

void UserInterface::showOptionsMenuView()
{
	this->swapView(&this->optionsMenuView);
}

void UserInterface::showPlayView()
{
	this->swapView(&this->playView);
}

void UserInterface::showRecordsView()
{
	this->swapView(&this->recordsView);
}

void UserInterface::swapView(View* view)
{
	this->currentView = view;
	this->refresh();
	this->game.refreshMousePosition();
}

void UserInterface::update(const double deltaTime)
{
	this->currentView->update(deltaTime);
}
