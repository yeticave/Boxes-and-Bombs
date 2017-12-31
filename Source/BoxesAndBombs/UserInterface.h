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

#ifndef IS_USERINTERFACE_INCLUDED
#define IS_USERINTERFACE_INCLUDED

#include "Camera.h"
#include "Gui/DifficultyMenuView.h"
#include "Gui/MainMenuView.h"
#include "Gui/OptionsMenuView.h"
#include "Gui/PlayView.h"
#include "Gui/RecordsView.h"
using namespace Gui;

class Game;

class UserInterface
{
	public:
	UserInterface(Game& game);
	void draw() const;
	bool handleKeyPressEvent(const int key, const int scancode, const int action, const int mods);
	bool handleMouseClickEvent(const int button, const int action);
	bool handleMouseMovementEvent(const float x, const float y);
	bool handleMouseScrollEvent(const float delta);
	void refresh();
	void showDifficultyMenuView();
	void showMainMenuView();
	void showOptionsMenuView();
	void showPlayView();
	void showRecordsView();
	void update(const double deltaTime);

	private:
	View* currentView;
	DifficultyMenuView difficultyMenuView;
	Game& game;
	MainMenuView mainMenuView;
	OptionsMenuView optionsMenuView;
	PlayView playView;
	RecordsView recordsView;
	void swapView(View* view);
};

#endif
