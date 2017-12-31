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

#ifndef IS_LABEL_INCLUDED
#define IS_LABEL_INCLUDED

#include "../main.h"
#include "../ResourceManager.h"
#include "../Configuration/Options.h"
#include "Pivot.h"
using namespace std;
using namespace Configuration;

namespace Gui
{
	class Label
	{
		public:
		array<float, 3> color;
		array<float, 4> boundaries;
		float height;
		bool isVisible;
		float width;

		Label(ResourceManager& resourceManager);
		virtual void draw() const;
		void draw(const array<float, 3>& color) const;
		void refresh();
		void setPosition(const float x, const float y, const Pivot pivot);
		void setScale(const float scale);
		void setText(const string& text);

		protected:
		static char fontSizing[103];
		int length;
		Pivot pivot;
		float scale;
		string text;
		static const TextureAtlas* texture;
		float* textureCoordinates;
		float* vertices;
		float x;
		float y;
		bool isDrawable(char character) const;
	};
}

#endif
