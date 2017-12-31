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

#include "Options.h"

namespace Configuration
{
	const int Options::version = 1;

	Options::Options()
	{
		this->reset();
	}

	void Options::load()
	{
		ifstream file;
		file.open((this->dataPath + "data.bin").c_str(), ios::binary);
		if (file.is_open())
		{
			int version;
			this->read(&file, (char*)&version, sizeof(version));

			if (version == 1)
			{
				char* outputs[] = {
					(char*)&this->doGimbalLockCamera,
					(char*)&this->doHideSaturatedIndicators,
					(char*)&this->doShrinkMarkedBoxes,
					(char*)&this->colorblindMode,
					(char*)&this->dragButton,
					(char*)&this->dragSensitivity,
					(char*)&this->dragThreshold,
					(char*)&this->soundVolume,
					(char*)&this->wireframingMode,
					(char*)&this->records[0],
					(char*)&this->records[1],
					(char*)&this->records[2],
				};

				int sizes[] = {
					sizeof(this->doGimbalLockCamera),
					sizeof(this->doHideSaturatedIndicators),
					sizeof(this->doShrinkMarkedBoxes),
					sizeof(this->colorblindMode),
					sizeof(this->dragButton),
					sizeof(this->dragSensitivity),
					sizeof(this->dragThreshold),
					sizeof(this->soundVolume),
					sizeof(this->wireframingMode),
					sizeof(this->records[0]),
					sizeof(this->records[1]),
					sizeof(this->records[2]),
				};

				for (int i = 0; i < 12; ++i)
				{
					if (!this->read(&file, outputs[i], sizes[i]))
					{
						this->reset();
						break;
					}
				}
			}
			else
			{
				this->reset();
			}
		}
		file.close();
	}

	bool Options::read(ifstream* file, char* output, int size) const
	{
		bool result = file->rdbuf()->sgetn(output, size) == size;
		return result;
	}

	void Options::reset()
	{
		this->records[0] = -1.0;
		this->records[1] = -1.0;
		this->records[2] = -1.0;

		this->colorblindMode = ColorblindMode::colors;
		this->doGimbalLockCamera = true;
		this->doHideSaturatedIndicators = true;
		this->doShrinkMarkedBoxes = true;
		this->dragButton = DragButton::both;
		this->dragSensitivity = 5;
		this->dragThreshold = 25;
		this->soundVolume = 100;
		this->wireframingMode = WireframingMode::billboards;

		this->keyBindings.contractFields = GLFW_KEY_X;
		this->keyBindings.expandFields = GLFW_KEY_Z;
		this->keyBindings.panToBack = GLFW_KEY_3;
		this->keyBindings.panToBottom = GLFW_KEY_6;
		this->keyBindings.panToFront = GLFW_KEY_1;
		this->keyBindings.panToLeft = GLFW_KEY_4;
		this->keyBindings.panToPerspective = GLFW_KEY_7;
		this->keyBindings.panToRight = GLFW_KEY_2;
		this->keyBindings.panToTop = GLFW_KEY_5;
		this->keyBindings.pitchDown = GLFW_KEY_S;
		this->keyBindings.pitchUp = GLFW_KEY_W;
		this->keyBindings.quickRestart = GLFW_KEY_F2;
		this->keyBindings.rollClockwise = GLFW_KEY_E;
		this->keyBindings.rollCounterClockwise = GLFW_KEY_Q;
		this->keyBindings.toggleSaturatedIndicatorDisplay = GLFW_KEY_TAB;
		this->keyBindings.yawLeft = GLFW_KEY_A;
		this->keyBindings.yawRight = GLFW_KEY_D;
		this->keyBindings.zoomIn = GLFW_KEY_R;
		this->keyBindings.zoomOut = GLFW_KEY_F;
	}

	void Options::save() const
	{
		ofstream file;
		file.open((this->dataPath + "data.bin").c_str(), ios::binary | ios::trunc);
		if (file.is_open())
		{
			this->write(&file, (char*)&Options::version, sizeof(Options::version));
			this->write(&file, (char*)&this->doGimbalLockCamera, sizeof(this->doGimbalLockCamera));
			this->write(&file, (char*)&this->doHideSaturatedIndicators, sizeof(this->doHideSaturatedIndicators));
			this->write(&file, (char*)&this->doShrinkMarkedBoxes, sizeof(this->doShrinkMarkedBoxes));
			this->write(&file, (char*)&this->colorblindMode, sizeof(this->colorblindMode));
			this->write(&file, (char*)&this->dragButton, sizeof(this->dragButton));
			this->write(&file, (char*)&this->dragSensitivity, sizeof(this->dragSensitivity));
			this->write(&file, (char*)&this->dragThreshold, sizeof(this->dragThreshold));
			this->write(&file, (char*)&this->soundVolume, sizeof(this->soundVolume));
			this->write(&file, (char*)&this->wireframingMode, sizeof(this->wireframingMode));

			for (int i = 0; i < 3; ++i)
			{
				this->write(&file, (char*)&this->records[i], sizeof(this->records[i]));
			}
		}
		file.close();
	}

	void Options::write(ofstream* file, char* input, int size) const
	{
		for (int i = 0; i < size; ++i)
		{
			file->rdbuf()->sputn(&input[i], 1);
		}
	}
}
