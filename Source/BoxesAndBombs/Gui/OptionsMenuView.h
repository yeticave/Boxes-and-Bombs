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

#ifndef IS_OPTIONSMENUVIEW_INCLUDED
#define IS_OPTIONSMENUVIEW_INCLUDED

#include "../Game.h"
#include "Button.h"
#include "View.h"

class Game;

namespace Gui
{
	class OptionsMenuView : public View
	{
		public:
		OptionsMenuView(Game& game);
		virtual void goBack() override;
		virtual bool handleKeyPressEvent(const int key, const int scancode, const int action, const int mods) override;
		virtual void refresh() override;
		virtual void update(const double deltaTime) override;

		private:
		Button backButton;
		int backButtonIndex;
		Button cameraModeButton;
		Label cameraModeLabel;
		Button colorblindModeButton;
		Label colorblindModeLabel;
		Button dragButtonButton;
		Label dragButtonLabel;
		Button dragSensitivityButton;
		Label dragSensitivityLabel;
		Button dragThresholdButton;
		Label dragThresholdLabel;
		Button displayIndicatorsButton;
		Label displayIndicatorsLabel;
		Label optionsLabel;
		Button shrinkMarkedBoxesButton;
		Label shrinkMarkedBoxesLabel;
		Button soundVolumeButton;
		Label soundVolumeLabel;
		Label tooltipLabel;
		Button wireframingModeButton;
		Label wireframingModeLabel;

		void cameraModeButtonClicked();
		void cameraModeButtonRightClicked();
		void colorblindModeButtonClicked();
		void colorblindModeButtonRightClicked();
		void dragButtonButtonClicked();
		void dragButtonButtonRightClicked();
		void dragSensitivityButtonClicked();
		void dragSensitivityButtonRightClicked();
		void dragThresholdButtonClicked();
		void dragThresholdButtonRightClicked();
		void displayIndicatorsButtonClicked();
		void displayIndicatorsButtonRightClicked();
		void shrinkMarkedBoxesButtonClicked();
		void shrinkMarkedBoxesButtonRightClicked();
		void soundVolumeButtonClicked();
		void soundVolumeButtonRightClicked();
		void wireframingModeButtonClicked();
		void wireframingModeButtonRightClicked();
	};
}

#endif
