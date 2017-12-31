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
#include "PlayView.h"

namespace Gui
{
	PlayView::PlayView(Game& game) : View(game), bombsLabel(game.resourceManager), gameOverLabel(game.resourceManager),
		playAgainButton(game.resourceManager), recordLabel(game.resourceManager), timeLabel(game.resourceManager)
	{
		this->labels.push_back(&this->bombsLabel);
		this->labels.push_back(&this->gameOverLabel);
		this->labels.push_back(&this->recordLabel);
		this->labels.push_back(&this->timeLabel);

		this->playAgainButton.setText("Play Again");
		this->playAgainButton.clicked = bind(&PlayView::playAgainButtonClicked, this);
		this->buttons.push_back(&this->playAgainButton);
	}

	void PlayView::playAgainButtonClicked()
	{
		this->game.newGame();
	}

	void PlayView::goBack()
	{
		this->game.userInterface.showMainMenuView();
	}

	bool PlayView::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods)
	{
		const KeyBindings& keyBindings = this->game.options.keyBindings;
		bool result = false;

		if (action == GLFW_PRESS)
		{
			if (key == keyBindings.quickRestart)
			{
				this->game.newGame();
				result = true;
			}
			else
			{
				result = View::handleKeyPressEvent(key, scancode, action, mods, false);
			}
		}

		return result;
	}

	bool PlayView::handleMouseClickEvent(const int button, const int action)
	{
		bool result = false;
		if (this->game.status == GameStatus::finished)
		{
			result = View::handleMouseClickEvent(button, action);
		}
		return result;
	}

	bool PlayView::handleMouseMovementEvent(const float x, const float y)
	{
		bool result = false;
		if (this->game.status == GameStatus::finished)
		{
			result = View::handleMouseMovementEvent(x, y);
		}
		return result;
	}

	bool PlayView::handleMouseScrollEvent(const float delta)
	{
		bool result = false;
		if (this->doDim)
		{
			result = View::handleMouseScrollEvent(delta);
		}
		return result;
	}

	void PlayView::refresh()
	{
		int lesser = min({ this->game.viewportWidth, this->game.viewportHeight });
		float fontScale = this->getFontScale();
		float padding = fontScale * this->paddingSize;
		float x = (float)this->game.viewportWidth / 2.0f;
		float y = (float)this->game.viewportHeight / 2.0f;

		this->bombsLabel.setScale(fontScale * this->smallFontSize);
		this->bombsLabel.setPosition(this->game.viewportWidth - padding, padding, Pivot::topRight);

		this->gameOverLabel.setScale(fontScale);
		this->gameOverLabel.setPosition(x, y - this->gameOverLabel.height - padding, Pivot::center);

		this->playAgainButton.setScale(fontScale);
		this->playAgainButton.setPosition(x, y + this->playAgainButton.height + padding, Pivot::center);

		this->recordLabel.setScale(fontScale);
		this->recordLabel.setPosition(x, y, Pivot::center);

		this->timeLabel.setScale(fontScale * this->smallFontSize);
		this->timeLabel.setPosition(padding, padding, Pivot::topLeft);

		View::refresh();
	}

	void PlayView::update(const double deltaTime)
	{
		string time = this->toTime(this->game.time);

		if (!this->doDim && this->game.status == GameStatus::finished)
		{
			string gameOverText = this->game.didWin ? "Victory!" : "GAME OVER";
			double record = this->game.options.records[(int)this->game.difficulty];
			string recordText;

			if (this->game.isNewRecord)
			{
				recordText = "New Record!";
			}
			else if (record > 0.0)
			{
				recordText = "Record:" + this->toTime(record);
			}

			this->doDim = true;
			this->gameOverLabel.isVisible = true;
			this->gameOverLabel.setText(gameOverText);
			this->recordLabel.isVisible = true;
			this->recordLabel.setText(recordText);
			this->playAgainButton.isVisible = true;
			this->game.scene.clearSelection();
			View::refresh();
		}
		else if (this->game.status != GameStatus::finished)
		{
			this->doDim = false;
			this->gameOverLabel.isVisible = false;
			this->recordLabel.isVisible = false;
			this->playAgainButton.isVisible = false;
			View::refresh();
		}

		this->bombsLabel.setText("Bombs:" + to_string(this->game.scene.bombsLeft));
		this->timeLabel.setText("Time:" + time);
		View::update(deltaTime);
	}
}
