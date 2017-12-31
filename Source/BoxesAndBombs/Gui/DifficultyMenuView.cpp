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

#include "../Game.h"
#include "DifficultyMenuView.h"

namespace Gui
{
	DifficultyMenuView::DifficultyMenuView(Game& game) : View(game), backButton(game.resourceManager),
		complexButton(game.resourceManager), difficultyLabel(game.resourceManager), mediumButton(game.resourceManager),
		simpleButton(game.resourceManager)
	{
		this->difficultyLabel.setText("Difficulty");
		this->labels.push_back(&this->difficultyLabel);

		this->simpleButton.setText("Simple");
		this->simpleButton.clicked = bind(&DifficultyMenuView::simpleButtonClicked, this);
		this->buttons.push_back(&this->simpleButton);

		this->mediumButton.setText("Medium");
		this->mediumButton.clicked = bind(&DifficultyMenuView::mediumButtonClicked, this);
		this->buttons.push_back(&this->mediumButton);

		this->complexButton.setText("Complex");
		this->complexButton.clicked = bind(&DifficultyMenuView::complexButtonClicked, this);
		this->buttons.push_back(&this->complexButton);

		this->backButton.setText("Back");
		this->backButton.clicked = bind(&DifficultyMenuView::goBack, this);
		this->buttons.push_back(&this->backButton);
	}

	void DifficultyMenuView::complexButtonClicked()
	{
		this->setDifficulty(Difficulty::complex);
	}

	void DifficultyMenuView::goBack()
	{
		this->game.userInterface.showMainMenuView();
	}

	void DifficultyMenuView::mediumButtonClicked()
	{
		this->setDifficulty(Difficulty::medium);
	}

	void DifficultyMenuView::refresh()
	{
		int lesser = min({ this->game.viewportWidth, this->game.viewportHeight });
		float fontScale = this->getFontScale();
		float padding = fontScale * this->paddingSize;
		float x = (float)this->game.viewportWidth / 2.0f;
		float y = (float)this->game.viewportHeight / 2.0f;

		this->difficultyLabel.setScale(fontScale * this->largeFontSize);
		this->difficultyLabel.setPosition(x, padding, Pivot::top);

		this->mediumButton.setScale(fontScale);
		this->mediumButton.setPosition(x, y, Pivot::center);

		this->simpleButton.setScale(fontScale);
		this->simpleButton.setPosition(x, this->mediumButton.boundaries[1] - padding, Pivot::bottom);

		this->complexButton.setScale(fontScale);
		this->complexButton.setPosition(x, this->mediumButton.boundaries[3] + padding, Pivot::top);

		this->backButton.setScale(fontScale * this->smallFontSize);
		this->backButton.setPosition(padding, this->game.viewportHeight - padding, Pivot::bottomLeft);

		View::refresh();
	}

	void DifficultyMenuView::setDifficulty(Difficulty difficulty)
	{
		this->game.userInterface.showPlayView();
		this->game.setDifficulty(difficulty);
	}

	void DifficultyMenuView::simpleButtonClicked()
	{
		this->setDifficulty(Difficulty::simple);
	}
}
