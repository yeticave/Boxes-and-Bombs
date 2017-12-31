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
#include "MainMenuView.h"

namespace Gui
{
	MainMenuView::MainMenuView(Game& game) : View(game), exitButton(game.resourceManager),
		optionsButton(game.resourceManager), playButton(game.resourceManager), recordsButton(game.resourceManager),
		titleLabel(game.resourceManager), versionLabel(game.resourceManager)
	{
		this->titleLabel.setText("Boxes and Bombs");
		this->labels.push_back(&this->titleLabel);

		this->playButton.setText("Play");
		this->playButton.clicked = bind(&MainMenuView::playButtonClicked, this);
		this->buttons.push_back(&this->playButton);

		this->recordsButton.setText("Records");
		this->recordsButton.clicked = bind(&MainMenuView::recordsButtonClicked, this);
		this->buttons.push_back(&this->recordsButton);

		this->optionsButton.setText("Options");
		this->optionsButton.clicked = bind(&MainMenuView::optionsButtonClicked, this);
		this->buttons.push_back(&this->optionsButton);

		this->exitButton.setText("Exit");
		this->exitButton.clicked = bind(&MainMenuView::exitButtonClicked, this);
		this->buttons.push_back(&this->exitButton);

		this->versionLabel.setText("Version 1.0.0");
		this->labels.push_back(&this->versionLabel);
	}

	void MainMenuView::goBack()
	{
		if (this->game.status == GameStatus::uninitialized)
		{
			this->game.shutdown();
		}
		else
		{
			this->game.userInterface.showPlayView();
		}
	}

	void MainMenuView::exitButtonClicked()
	{
		this->game.shutdown();
	}

	void MainMenuView::optionsButtonClicked()
	{
		this->game.userInterface.showOptionsMenuView();
	}

	void MainMenuView::playButtonClicked()
	{
		this->game.userInterface.showDifficultyMenuView();
	}

	void MainMenuView::recordsButtonClicked()
	{
		this->game.userInterface.showRecordsView();
	}

	void MainMenuView::refresh()
	{
		int lesser = min({ this->game.viewportWidth, this->game.viewportHeight });
		float fontScale = this->getFontScale();
		float padding = fontScale * this->paddingSize;
		float x = (float)this->game.viewportWidth / 2.0f;
		float y = (float)this->game.viewportHeight / 2.0f;

		this->titleLabel.setScale(fontScale * this->largeFontSize);
		this->titleLabel.setPosition(x, padding, Pivot::top);

		this->recordsButton.setScale(fontScale);
		this->recordsButton.setPosition(x, y - padding / 2.0f, Pivot::bottom);

		this->optionsButton.setScale(fontScale);
		this->optionsButton.setPosition(x, y + padding / 2.0f, Pivot::top);

		this->playButton.setScale(fontScale);
		this->playButton.setPosition(x, this->recordsButton.boundaries[1] - padding, Pivot::bottom);

		this->exitButton.setScale(fontScale);
		this->exitButton.setPosition(x, this->optionsButton.boundaries[3] + padding, Pivot::top);

		this->versionLabel.setScale(fontScale * this->smallFontSize);
		this->versionLabel.setPosition(padding, this->game.viewportHeight - padding, Pivot::bottomLeft);

		View::refresh();
	}
}
