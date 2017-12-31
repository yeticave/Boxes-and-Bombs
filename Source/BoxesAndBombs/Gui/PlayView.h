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

#ifndef IS_PLAYVIEW_INCLUDED
#define IS_PLAYVIEW_INCLUDED

#include "../Game.h"
#include "Label.h"
#include "View.h"

class Game;

namespace Gui
{
	class PlayView : public View
	{
		public:
		PlayView(Game& game);
		virtual void goBack() override;
		virtual bool handleKeyPressEvent(const int key, const int scancode, const int action, const int mods) override;
		virtual bool handleMouseClickEvent(const int button, const int action) override;
		virtual bool handleMouseMovementEvent(const float x, const float y) override;
		virtual bool handleMouseScrollEvent(const float delta) override;
		void playAgainButtonClicked();
		virtual void refresh() override;
		virtual void update(const double deltaTime) override;

		private:
		Label bombsLabel;
		Label gameOverLabel;
		Button playAgainButton;
		Label recordLabel;
		Label timeLabel;
	};
}

#endif
