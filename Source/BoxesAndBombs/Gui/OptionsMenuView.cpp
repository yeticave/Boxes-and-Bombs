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
#include "OptionsMenuView.h"

namespace Gui
{
	OptionsMenuView::OptionsMenuView(Game& game) : View(game), backButton(game.resourceManager),
		cameraModeButton(game.resourceManager), cameraModeLabel(game.resourceManager),
		colorblindModeButton(game.resourceManager), colorblindModeLabel(game.resourceManager),
		displayIndicatorsButton(game.resourceManager), displayIndicatorsLabel(game.resourceManager),
		dragButtonButton(game.resourceManager), dragButtonLabel(game.resourceManager),
		dragSensitivityButton(game.resourceManager), dragSensitivityLabel(game.resourceManager),
		dragThresholdButton(game.resourceManager), dragThresholdLabel(game.resourceManager),
		optionsLabel(game.resourceManager), shrinkMarkedBoxesButton(game.resourceManager),
		shrinkMarkedBoxesLabel(game.resourceManager), soundVolumeButton(game.resourceManager),
		soundVolumeLabel(game.resourceManager), tooltipLabel(game.resourceManager),
		wireframingModeButton(game.resourceManager), wireframingModeLabel(game.resourceManager)
	{
		this->optionsLabel.setText("Options");
		this->labels.push_back(&this->optionsLabel);

		this->soundVolumeButton.setText("Sound Volume");
		this->soundVolumeButton.clicked = bind(&OptionsMenuView::soundVolumeButtonClicked, this);
		this->soundVolumeButton.rightClicked = bind(&OptionsMenuView::soundVolumeButtonRightClicked, this);
		this->buttons.push_back(&this->soundVolumeButton);
		this->labels.push_back(&this->soundVolumeLabel);

		this->dragButtonButton.setText("Camera Drag Button");
		this->dragButtonButton.clicked = bind(&OptionsMenuView::dragButtonButtonClicked, this);
		this->dragButtonButton.rightClicked = bind(&OptionsMenuView::dragButtonButtonRightClicked, this);
		this->buttons.push_back(&this->dragButtonButton);
		this->labels.push_back(&this->dragButtonLabel);

		this->dragThresholdButton.setText("    Threshold");
		this->dragThresholdButton.clicked = bind(&OptionsMenuView::dragThresholdButtonClicked, this);
		this->dragThresholdButton.rightClicked = bind(&OptionsMenuView::dragThresholdButtonRightClicked, this);
		this->buttons.push_back(&this->dragThresholdButton);
		this->labels.push_back(&this->dragThresholdLabel);

		this->dragSensitivityButton.setText("    Sensitivity");
		this->dragSensitivityButton.clicked = bind(&OptionsMenuView::dragSensitivityButtonClicked, this);
		this->dragSensitivityButton.rightClicked = bind(&OptionsMenuView::dragSensitivityButtonRightClicked, this);
		this->buttons.push_back(&this->dragSensitivityButton);
		this->labels.push_back(&this->dragSensitivityLabel);

		this->displayIndicatorsButton.setText("Show Saturated Indicators");
		this->displayIndicatorsButton.clicked = bind(&OptionsMenuView::displayIndicatorsButtonClicked, this);
		this->displayIndicatorsButton.rightClicked = bind(&OptionsMenuView::displayIndicatorsButtonRightClicked, this);
		this->buttons.push_back(&this->displayIndicatorsButton);
		this->labels.push_back(&this->displayIndicatorsLabel);

		this->shrinkMarkedBoxesButton.setText("Shrink Marked Boxes");
		this->shrinkMarkedBoxesButton.clicked = bind(&OptionsMenuView::shrinkMarkedBoxesButtonClicked, this);
		this->shrinkMarkedBoxesButton.rightClicked = bind(&OptionsMenuView::shrinkMarkedBoxesButtonRightClicked, this);
		this->buttons.push_back(&this->shrinkMarkedBoxesButton);
		this->labels.push_back(&this->shrinkMarkedBoxesLabel);

		this->wireframingModeButton.setText("Wireframing Mode");
		this->wireframingModeButton.clicked = bind(&OptionsMenuView::wireframingModeButtonClicked, this);
		this->wireframingModeButton.rightClicked = bind(&OptionsMenuView::wireframingModeButtonRightClicked, this);
		this->buttons.push_back(&this->wireframingModeButton);
		this->labels.push_back(&this->wireframingModeLabel);

		this->cameraModeButton.setText("Camera Mode");
		this->cameraModeButton.clicked = bind(&OptionsMenuView::cameraModeButtonClicked, this);
		this->cameraModeButton.rightClicked = bind(&OptionsMenuView::cameraModeButtonRightClicked, this);
		this->buttons.push_back(&this->cameraModeButton);
		this->labels.push_back(&this->cameraModeLabel);

		this->colorblindModeButton.setText("Colourblind Mode");
		this->colorblindModeButton.clicked = bind(&OptionsMenuView::colorblindModeButtonClicked, this);
		this->colorblindModeButton.rightClicked = bind(&OptionsMenuView::colorblindModeButtonRightClicked, this);
		this->buttons.push_back(&this->colorblindModeButton);
		this->labels.push_back(&this->colorblindModeLabel);

		this->labels.push_back(&this->tooltipLabel);
		this->backButtonIndex = this->buttons.size();
		this->backButton.setText("Back");
		this->backButton.clicked = bind(&OptionsMenuView::goBack, this);
		this->buttons.push_back(&this->backButton);
	}

	void OptionsMenuView::cameraModeButtonClicked()
	{
		this->game.toggleCameraMode(true);
	}

	void OptionsMenuView::cameraModeButtonRightClicked()
	{
		this->game.toggleCameraMode(false);
	}

	void OptionsMenuView::colorblindModeButtonClicked()
	{
		this->game.toggleColorblindMode(true);
	}

	void OptionsMenuView::colorblindModeButtonRightClicked()
	{
		this->game.toggleColorblindMode(false);
	}

	void OptionsMenuView::dragButtonButtonClicked()
	{
		this->game.toggleDragButton(true);
	}

	void OptionsMenuView::dragButtonButtonRightClicked()
	{
		this->game.toggleDragButton(false);
	}

	void OptionsMenuView::dragSensitivityButtonClicked()
	{
		this->game.toggleDragSensitivity(true);
	}

	void OptionsMenuView::dragSensitivityButtonRightClicked()
	{
		this->game.toggleDragSensitivity(false);
	}

	void OptionsMenuView::dragThresholdButtonClicked()
	{
		this->game.toggleDragThreshold(true);
	}

	void OptionsMenuView::dragThresholdButtonRightClicked()
	{
		this->game.toggleDragThreshold(false);
	}

	void OptionsMenuView::displayIndicatorsButtonClicked()
	{
		this->game.toggleSaturatedIndicatorDisplay(true);
	}

	void OptionsMenuView::displayIndicatorsButtonRightClicked()
	{
		this->game.toggleSaturatedIndicatorDisplay(false);
	}

	void OptionsMenuView::goBack()
	{
		this->game.userInterface.showMainMenuView();
	}

	bool OptionsMenuView::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods)
	{
		bool result = false;

		if (this->selectedButtonIndex != this->backButtonIndex && action == GLFW_PRESS)
		{
			switch (key)
			{
				case GLFW_KEY_LEFT:
					this->clickSelectedButton(GLFW_MOUSE_BUTTON_RIGHT);
					result = true;
					break;

				case GLFW_KEY_RIGHT:
					this->clickSelectedButton(GLFW_MOUSE_BUTTON_LEFT);
					result = true;
					break;
			}
		}

		if (!result)
		{
			result = View::handleKeyPressEvent(key, scancode, action, mods);
		}

		return result;
	}

	void OptionsMenuView::shrinkMarkedBoxesButtonClicked()
	{
		this->game.toggleMarkingMode(true);
	}

	void OptionsMenuView::shrinkMarkedBoxesButtonRightClicked()
	{
		this->game.toggleMarkingMode(false);
	}

	void OptionsMenuView::soundVolumeButtonClicked()
	{
		this->game.toggleSoundVolume(true);
	}

	void OptionsMenuView::soundVolumeButtonRightClicked()
	{
		this->game.toggleSoundVolume(false);
	}

	void OptionsMenuView::refresh()
	{
		int lesser = min({ this->game.viewportWidth, this->game.viewportHeight });
		float fontScale = this->getFontScale();
		float padding = fontScale * this->paddingSize;
		float buttonOffset = (this->game.viewportWidth - lesser) / 2.0f + padding;
		float labelOffset = this->game.viewportWidth - buttonOffset;
		float x = (float)this->game.viewportWidth / 2.0f;

		Button* buttons[] = {
			&this->soundVolumeButton,
			&this->dragButtonButton,
			&this->dragThresholdButton,
			&this->dragSensitivityButton,
			&this->displayIndicatorsButton,
			&this->shrinkMarkedBoxesButton,
			&this->wireframingModeButton,
			&this->cameraModeButton,
			&this->colorblindModeButton,
		};

		Label* labels[] = {
			&this->soundVolumeLabel,
			&this->dragButtonLabel,
			&this->dragThresholdLabel ,
			&this->dragSensitivityLabel,
			&this->displayIndicatorsLabel ,
			&this->shrinkMarkedBoxesLabel,
			&this->wireframingModeLabel,
			&this->cameraModeLabel,
			&this->colorblindModeLabel,
		};

		string dragButtonName;
		switch (this->game.options.dragButton)
		{
			case DragButton::both:
				dragButtonName = "Both";
				break;

			case DragButton::left:
				dragButtonName = "Left Button";
				break;

			case DragButton::right:
				dragButtonName = "Right Button";
				break;

			default:
				dragButtonName = "None";
				break;
		}

		string wireframingModeName;
		switch (this->game.options.wireframingMode)
		{
			case WireframingMode::all:
				wireframingModeName = "All Fields";
				break;

			case WireframingMode::billboards:
				wireframingModeName = "Indicator Fields";
				break;

			default:
				wireframingModeName = "None";
				break;
		}

		string colorblindModeName;
		switch (this->game.options.colorblindMode)
		{
			case ColorblindMode::colors:
				colorblindModeName = "Use Colours";
				break;

			case ColorblindMode::symbols:
				colorblindModeName = "Use Symbols";
				break;

			default:
				colorblindModeName = "Use Colours & Symbols";
				break;
		}

		this->optionsLabel.setScale(fontScale * this->largeFontSize);
		this->optionsLabel.setPosition(x, padding, Pivot::top);

		this->soundVolumeLabel.setText(to_string(this->game.options.soundVolume));
		this->dragButtonLabel.setText(dragButtonName);
		this->dragThresholdLabel.setText(to_string(this->game.options.dragThreshold));
		this->dragSensitivityLabel.setText(to_string(this->game.options.dragSensitivity));
		this->displayIndicatorsLabel.setText(this->game.options.doHideSaturatedIndicators ? "No" : "Yes");
		this->shrinkMarkedBoxesLabel.setText(this->game.options.doShrinkMarkedBoxes ? "Yes" : "No");
		this->wireframingModeLabel.setText(wireframingModeName);
		this->cameraModeLabel.setText(this->game.options.doGimbalLockCamera ? "Gimbal Locked" : "Free");
		this->colorblindModeLabel.setText(colorblindModeName);

		float rowOffset = this->optionsLabel.boundaries[3] + padding;
		for (int i = 0; i < 8; ++i)
		{
			buttons[i]->setScale(fontScale * this->smallFontSize);
			buttons[i]->setPosition(buttonOffset, rowOffset, Pivot::topLeft);
			labels[i]->setScale(fontScale * this->smallFontSize);
			labels[i]->setPosition(labelOffset, rowOffset, Pivot::topRight);
			rowOffset = buttons[i]->boundaries[3] + padding;
		}

		this->tooltipLabel.setScale(fontScale * this->smallFontSize);
		this->tooltipLabel.setPosition(x, rowOffset + padding * 3.0f, Pivot::top);
		this->backButton.setScale(fontScale * this->smallFontSize);
		this->backButton.setPosition(padding, this->game.viewportHeight - padding, Pivot::bottomLeft);
	}

	void OptionsMenuView::update(const double deltaTime)
	{
		if (this->selectedButtonIndex >= 0 && this->selectedButtonIndex < 8)
		{
			string tooltips[] = {
				"",
				"Control the camera by holding down the\nselected mouse button.",
				"The number of pixels the cursor has to\ntravel before starting to drag the camera.",
				"The distance the camera will be dragged\nrelative to the mouse movement.",
				"Hide the indicators that have the correct\nnumber of boxes marked around them.",
				"Shrink the marked boxes to improve the\nvisibility of the rest of the grid.",
				"Draw a wireframe in place of the revealed\nboxes.",
				"Limit the axes the camera can rotate on.",
				"Circumvent the dependency on colours by\nusing symbols instead.",
			};
			this->tooltipLabel.isVisible = true;
			this->tooltipLabel.setText(tooltips[this->selectedButtonIndex]);
		}
		else
		{
			this->tooltipLabel.isVisible = false;
		}
		View::update(deltaTime);
	}

	void OptionsMenuView::wireframingModeButtonClicked()
	{
		this->game.toggleWireframingMode(true);
	}

	void OptionsMenuView::wireframingModeButtonRightClicked()
	{
		this->game.toggleWireframingMode(false);
	}
}
