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
#include "RecordsView.h"

namespace Gui
{
	RecordsView::RecordsView(Game& game) : View(game), backButton(game.resourceManager),
		complexLabel(game.resourceManager), complexTimeLabel(game.resourceManager), mediumLabel(game.resourceManager),
		mediumTimeLabel(game.resourceManager), recordsLabel(game.resourceManager), simpleLabel(game.resourceManager),
		simpleTimeLabel(game.resourceManager)
	{
		this->recordsLabel.setText("Records");
		this->labels.push_back(&this->recordsLabel);

		this->simpleLabel.setText("Simple");
		this->labels.push_back(&this->simpleLabel);
		this->labels.push_back(&this->simpleTimeLabel);

		this->mediumLabel.setText("Medium");
		this->labels.push_back(&this->mediumLabel);
		this->labels.push_back(&this->mediumTimeLabel);

		this->complexLabel.setText("Complex");
		this->labels.push_back(&this->complexLabel);
		this->labels.push_back(&this->complexTimeLabel);

		this->backButton.setText("Back");
		this->backButton.clicked = bind(&RecordsView::goBack, this);
		this->buttons.push_back(&this->backButton);
	}

	void RecordsView::goBack()
	{
		this->game.userInterface.showMainMenuView();
	}

	void RecordsView::refresh()
	{
		int lesser = min({ this->game.viewportWidth, this->game.viewportHeight });
		float fontScale = this->getFontScale();
		float padding = fontScale * this->paddingSize;
		float x = (float)this->game.viewportWidth / 2.0f;
		float y = (float)this->game.viewportHeight / 2.0f;

		this->recordsLabel.setScale(fontScale * this->largeFontSize);
		this->recordsLabel.setPosition(x, padding, Pivot::top);

		this->simpleTimeLabel.setText(this->toTime(this->game.options.records[0]));
		this->mediumTimeLabel.setText(this->toTime(this->game.options.records[1]));
		this->complexTimeLabel.setText(this->toTime(this->game.options.records[2]));

		this->simpleLabel.setScale(fontScale);
		this->simpleTimeLabel.setScale(fontScale);
		this->mediumLabel.setScale(fontScale);
		this->mediumTimeLabel.setScale(fontScale);
		this->complexLabel.setScale(fontScale);
		this->complexTimeLabel.setScale(fontScale);

		float simpleWidth = this->simpleLabel.width + this->simpleTimeLabel.width;
		float mediumWidth = this->mediumLabel.width + this->mediumTimeLabel.width;
		float complexWidth = this->complexLabel.width + this->complexTimeLabel.width;
		float maxWidth = max({ simpleWidth, mediumWidth, complexWidth });
		float offset = maxWidth / 2.0f;

		this->mediumLabel.setPosition(x - offset, y, Pivot::left);
		this->mediumTimeLabel.setPosition(x + offset, y, Pivot::right);

		this->simpleLabel.setPosition(x - offset, this->mediumLabel.boundaries[1] - padding, Pivot::bottomLeft);
		this->simpleTimeLabel.setPosition(x + offset, this->simpleLabel.boundaries[3], Pivot::bottomRight);

		this->complexLabel.setPosition(x - offset, this->mediumLabel.boundaries[3] + padding, Pivot::topLeft);
		this->complexTimeLabel.setPosition(x + offset, complexLabel.boundaries[3], Pivot::bottomRight);

		this->backButton.setScale(fontScale * this->smallFontSize);
		this->backButton.setPosition(padding, this->game.viewportHeight - padding, Pivot::bottomLeft);

		View::refresh();
	}
}
