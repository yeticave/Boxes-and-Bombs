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
#include "View.h"

namespace Gui
{
	int View::clickSound;
	int View::hoverSound;
	bool View::isInitialized = false;

	View::View(Game& game) : game(game)
	{
		if (!View::isInitialized)
		{
			View::clickSound = game.resourceManager.getSound("click");
			View::hoverSound = game.resourceManager.getSound("hover");
			View::isInitialized = true;
		}

		this->doDim = true;
		this->largeFontSize = 1.2f;
		this->paddingSize = 1.0f / 16.0f;
		this->selectedButtonIndex = -1;
		this->smallFontSize = 0.6f;
	}

	bool View::clickSelectedButton(const int button)
	{
		bool result = false;

		if (this->selectedButtonIndex >= 0)
		{
			Button* selectedButton = this->buttons[this->selectedButtonIndex];
			if (button == GLFW_MOUSE_BUTTON_LEFT && selectedButton->clicked)
			{
				selectedButton->clicked();
				result = true;
			}
			else if (button == GLFW_MOUSE_BUTTON_RIGHT && selectedButton->rightClicked)
			{
				selectedButton->rightClicked();
				result = true;
			}

			this->game.resourceManager.playSound(View::clickSound);
		}

		return result;
	}

	void View::draw() const
	{
		if (this->doDim)
		{
			glBegin(GL_QUADS);
			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
			glVertex2i(0, this->game.viewportHeight);
			glVertex2i(this->game.viewportWidth, this->game.viewportHeight);
			glVertex2i(this->game.viewportWidth, 0);
			glVertex2i(0, 0);
			glEnd();
		}

		int buttonCount = this->buttons.size();
		for (int i = 0; i < buttonCount; ++i)
		{
			if (this->buttons[i]->isVisible)
			{
				this->buttons[i]->draw();
			}
		}

		int labelCount = this->labels.size();
		for (int i = 0; i < labelCount; ++i)
		{
			if (this->labels[i]->isVisible)
			{
				this->labels[i]->draw();
			}
		}
	}

	float View::getFontScale() const
	{
		int lesser = min({ this->game.viewportWidth, this->game.viewportHeight });
		float result = (float)lesser / 8.0f;
		return result;
	}

	bool View::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods)
	{
		bool result = this->handleKeyPressEvent(key, scancode, action, mods, true);
		return result;
	}

	bool View::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods,
		const bool defaultResult)
	{
		const KeyBindings& keyBindings = this->game.options.keyBindings;
		bool result = defaultResult;

		if (action == GLFW_PRESS)
		{
			switch (key)
			{
				case GLFW_KEY_ENTER:
				case GLFW_KEY_SPACE:
					result = this->clickSelectedButton(GLFW_MOUSE_BUTTON_LEFT);
					break;

				case GLFW_KEY_ESCAPE:
					this->game.scene.clearSelection();
					this->goBack();
					result = true;
					break;

				case GLFW_KEY_DOWN:
					this->selectNextButton();
					result = true;
					break;

				case GLFW_KEY_UP:
					this->selectPreviousButton();
					result = true;
					break;

				default:
					if (key == keyBindings.toggleSaturatedIndicatorDisplay)
					{
						this->game.toggleSaturatedIndicatorDisplay(true);
						result = true;
					}
					break;
			}
		}

		return result;
	}

	bool View::handleMouseClickEvent(const int button, const int action)
	{
		if (action == GLFW_RELEASE)
		{
			int buttonCount = this->buttons.size();

			for (int i = 0; i < buttonCount; ++i)
			{
				if (this->buttons[i]->isWithinBoundaries(this->game.mouseX, this->game.mouseY))
				{
					this->clickSelectedButton(button);
					break;
				}
			}
		}
		return true;
	}

	bool View::handleMouseMovementEvent(const float x, const float y)
	{
		int buttonCount = this->buttons.size();
		bool isHovering = false;

		for (int i = 0; i < buttonCount; ++i)
		{
			if (this->buttons[i]->isWithinBoundaries(this->game.mouseX, this->game.mouseY))
			{
				isHovering = true;
				this->selectButton(i);
				break;
			}
		}

		if (!isHovering)
		{
			this->selectButton(-1);
		}

		return true;
	}

	bool View::handleMouseScrollEvent(const float delta)
	{
		if (delta < 0.0)
		{
			this->selectNextButton();
		}
		else if (delta > 0.0)
		{
			this->selectPreviousButton();
		}
		return true;
	}

	void View::refresh()
	{
		this->handleMouseMovementEvent(this->game.mouseX, this->game.mouseY);
	}

	void View::selectButton(const int index)
	{
		int buttonCount = this->buttons.size();

		for (int i = 0; i < buttonCount; ++i)
		{
			this->buttons[i]->isHovered = false;
		}

		if (index >= 0)
		{
			this->buttons[index]->isHovered = true;
		}

		this->selectedButtonIndex = index;
	}

	void View::selectNextButton()
	{
		int index = this->selectedButtonIndex < 0 ? 0 : ++this->selectedButtonIndex % this->buttons.size();
		this->selectButton(index);
	}

	void View::selectPreviousButton()
	{
		int index = --this->selectedButtonIndex;
		if (index < 0)
		{
			index = this->buttons.size() - 1;
		}
		this->selectButton(index);
	}

	string View::toTime(const double time) const
	{
		string result;

		if (time < 0.0)
		{
			result = "N/A";
		}
		else
		{
			auto minutes = (int)floor(time / 60.0);
			double seconds = time - minutes * 60.0;
			string secondsString = to_string(seconds);
			int pos = secondsString.find_first_of('.');
			result = secondsString.substr(0, pos + 2);
			if (seconds < 10.0)
			{
				result = "0" + result;
			}
			if (minutes > 0)
			{
				result = to_string(minutes) + ":" + result;
				if (minutes < 10)
				{
					result = "0" + result;
				}
			}
		}

		return result;
	}

	void View::update(const double deltaTime)
	{
		static int lastButtonIndex = this->selectedButtonIndex;
		if (this->selectedButtonIndex != lastButtonIndex)
		{
			this->game.resourceManager.playSound(View::hoverSound);
			lastButtonIndex = this->selectedButtonIndex;
		}
	}
}
