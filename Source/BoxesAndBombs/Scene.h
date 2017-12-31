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

#ifndef IS_SCENE_INCLUDED
#define IS_SCENE_INCLUDED

#include "main.h"
#include "Camera.h"
#include "Field.h"
#include "Math/Ray.h"
using namespace std;
using namespace Math;

class Game;

class Scene
{
	public:
	int bombsLeft;
	Scene(Game& game, Camera& camera);
	void clearSelection();
	void draw();
	bool handleKeyPressEvent(const int key, const int scancode, const int action, const int mods);
	bool handleMouseClickEvent(const int button, const int action);
	bool handleMouseMovementEvent(const float x, const float y);
	bool handleMouseScrollEvent(const float delta);
	void refresh();
	void reset();
	void setSize(const int width, const int height, const int depth, const int bombCount);
	void update(const double deltaTime);

	private:
	bool areFieldsBombed;
	int bombCount;
	Camera& camera;
	int depth;
	bool doContractFields;
	bool doExpandFields;
	bool doPanToBack;
	bool doPanToBottom;
	bool doPanToFront;
	bool doPanToLeft;
	bool doPanToPerspective;
	bool doPanToRight;
	bool doPanToTop;
	bool doPitchDown;
	bool doPitchUp;
	bool doReset;
	bool doRollClockwise;
	bool doRollCounterClockwise;
	bool doUpdateMouseRay;
	bool doUpdateSelection;
	bool doYawLeft;
	bool doYawRight;
	bool doZoomIn;
	bool doZoomOut;
	float dragLastX;
	float dragLastY;
	int dragMouseButton;
	float fieldDistance;
	float fieldSize;
	vector<Field> fields;
	Game& game;
	int height;
	bool isDraggingCamera;
	bool isMouseActive;
	int markSound;
	Ray mouseRay;
	int revealSound;
	int selectedFieldIndex;
	int unmarkSound;
	int width;

	bool areValidCoordinates(const int x, const int y, const int z) const;
	void bombFields(const int index);
	float calculateDelta(const bool doIncrease, const bool doDecrease, const float currentDelta, const double deltaTime,
		const float acceleration, const float maxDelta);
	bool checkVictory();
	Field* getField(const int x, const int y, const int z);
	array<int, 3> getFieldCoordinates(const int index) const;
	int getFieldIndex(const int x, const int y, const int z) const;
	int getSelectedFieldIndex();
	int getTotalFieldCount() const;
	void incrementNeighbourBombCount(const int x, const int y, const int z);
	void incrementNeighbourMarkCount(const int x, const int y, const int z, const int delta);
	bool markSelectedField();
	void repositionFields();
	void revealField(const int x, const int y, const int z);
	void revealField(const int index);
	bool revealSelectedField();
	void startDragging(const int button);
	void stopDragging(const int button);
	void updateCameraDistance();
	void updateMouseRay();
	void updateView();
};

#endif
