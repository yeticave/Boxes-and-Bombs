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

#include "Game.h"
#include "GameStatus.h"
#include "Math/Algorithms.h"
#include "Math/PerspectiveFrustrum.h"
#include "Math/RandomNumberGenerator.h"
#include "Scene.h"

Scene::Scene(Game& game, Camera& camera) : game(game), camera(camera)
{
	this->doContractFields = false;
	this->doExpandFields = false;
	this->doPitchDown = false;
	this->doPitchUp = false;
	this->doRollClockwise = false;
	this->doRollCounterClockwise = false;
	this->doUpdateMouseRay = false;
	this->doUpdateSelection = false;
	this->doYawLeft = false;
	this->doYawRight = false;
	this->dragMouseButton = -1;
	this->fieldDistance = 1.0f;
	this->fieldSize = this->fieldDistance;
	this->isMouseActive = true;
	this->bombCount = 0;
	this->selectedFieldIndex = -1;
	this->width = 0;
	this->height = 0;
	this->depth = 0;

	this->markSound = game.resourceManager.getSound("mark");
	this->revealSound = game.resourceManager.getSound("reveal");
	this->unmarkSound = game.resourceManager.getSound("unmark");
}

bool Scene::areValidCoordinates(const int x, const int y, const int z) const
{
	bool result = x >= 0 && x < this->width && y >= 0 && y < this->height && z >= 0 && z < this->depth;
	return result;
}

void Scene::bombFields(const int index)
{
	array<int, 3> coordinates = this->getFieldCoordinates(index);
	int bombedFields = 0;
	int protectedFields = 1;
	RandomNumberGenerator rng;
	int totalFieldCount = this->getTotalFieldCount() - 1;

	this->fields[index].isProtected = true;
	while (protectedFields < 8)
	{
		int x = coordinates[0] + rng.next(0, 2) - 1;
		int y = coordinates[1] + rng.next(0, 2) - 1;
		int z = coordinates[2] + rng.next(0, 2) - 1;

		if (this->areValidCoordinates(x, y, z))
		{
			Field* field = this->getField(x, y, z);
			if (!field->isProtected)
			{
				field->isProtected = true;
				++protectedFields;
			}
		}
	}

	while (bombedFields < this->bombCount)
	{
		int x = rng.next(0, this->width - 1);
		int y = rng.next(0, this->height - 1);
		int z = rng.next(0, this->depth - 1);
		Field* field = this->getField(x, y, z);

		if (!field->isBombed && !field->isProtected)
		{
			field->isBombed = true;
			bombedFields += 1;
			this->incrementNeighbourBombCount(x - 1, y, z);
			this->incrementNeighbourBombCount(x + 1, y, z);
			this->incrementNeighbourBombCount(x, y - 1, z);
			this->incrementNeighbourBombCount(x, y + 1, z);
			this->incrementNeighbourBombCount(x, y, z - 1);
			this->incrementNeighbourBombCount(x, y, z + 1);
		}
	}
}

float Scene::calculateDelta(const bool doIncrease, const bool doDecrease, const float currentDelta,
	const double deltaTime, const float acceleration, const float maxDelta)
{
	float result = currentDelta;

	if (doIncrease)
	{
		result += acceleration * (float)deltaTime;
		if (result > maxDelta)
		{
			result = maxDelta;
		}
	}
	else if (result > 0.0f)
	{
		result = 0.0f;
	}

	if (doDecrease)
	{
		result -= acceleration * (float)deltaTime;
		if (result < -maxDelta)
		{
			result = -maxDelta;
		}
	}
	else if (result < 0.0f)
	{
		result = 0.0f;
	}

	return result;
}

bool Scene::checkVictory()
{
	int markedFields = 0;
	int totalFieldCount = this->getTotalFieldCount();
	bool result = true;

	for (int i = 0; i < totalFieldCount; ++i)
	{
		Field* field = &fields.data()[i];
		if ((!field->isMarked && !field->isRevealed) || (field->isMarked && !field->isBombed))
		{
			result = false;
			break;
		}
	}

	if (result)
	{
		this->game.finishGame(true);
		for (int i = 0; i < totalFieldCount; ++i)
		{
			Field* field = &fields.data()[i];
			field->neighbourMarkCount = 0;
		}
	}

	return result;
}

void Scene::clearSelection()
{
	this->isMouseActive = false;
	this->doUpdateSelection = true;
	this->selectedFieldIndex = -1;
}

void Scene::draw()
{
	double zNear = 0.1;
	double zFar = 500.0;
	float aspectRatio = this->game.getAspectRatio();
	PerspectiveFrustrum frustrum = Algorithms::perspectiveFrustrum(45.0, aspectRatio, zNear, zFar);
	array<float, 16> matrix = Algorithms::cameraMatrix(this->camera.eye, this->camera.focus, this->camera.up);

	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(-frustrum.width, frustrum.width, -frustrum.height, frustrum.height, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMultMatrixf(matrix.data());

	int totalFieldCount = this->getTotalFieldCount();
	for (int i = 0; i < totalFieldCount; ++i)
	{
		Field* field = &this->fields[i];
		field->draw(this->camera);
	}

	if (this->doUpdateMouseRay)
	{
		Vector direction = Algorithms::worldPosition(this->game.mouseX, this->game.mouseY, 1.0f);
		Vector origin = Algorithms::worldPosition(this->game.mouseX, this->game.mouseY, 0.0f);
		direction = direction - origin;
		direction.normalize();
		this->mouseRay.direction = direction;
		this->mouseRay.origin = origin;
		this->doUpdateSelection = true;
		this->doUpdateMouseRay = false;
	}
}

Field* Scene::getField(const int x, const int y, const int z)
{
	int index = this->getFieldIndex(x, y, z);
	Field* result = &this->fields.data()[index];
	return result;
}

array<int, 3> Scene::getFieldCoordinates(const int index) const
{
	int zOffset = this->width * this->height;
	int x = index % zOffset % this->width;
	int y = (index % zOffset - x) / this->width;
	int z = (index - y * this->width - x) / zOffset;
	array<int, 3> result = { x, y, z };
	return result;
}

int Scene::getFieldIndex(const int x, const int y, const int z) const
{
	int result = z * this->width * this->height + y * this->width + x;
	return result;
}

int Scene::getSelectedFieldIndex()
{
	float minDistance = -1.0f;
	int totalFieldCount = this->getTotalFieldCount();
	int result = -1;

	for (int i = 0; i < this->width; ++i)
	{
		for (int j = 0; j < this->height; ++j)
		{
			for (int k = 0; k < this->depth; ++k)
			{
				int index = this->getFieldIndex(i, j, k);
				Field* field = &this->fields.data()[index];
				if (!field->isRevealed)
				{
					float distance = field->intersect(this->mouseRay);
					if (distance > 0.0f && (minDistance < 0.0f || distance < minDistance))
					{
						minDistance = distance;
						result = index;
					}
				}
			}
		}
	}

	return result;
}

int Scene::getTotalFieldCount() const
{
	int result = this->width * this->height * this->depth;
	return result;
}

bool Scene::handleKeyPressEvent(const int key, const int scancode, const int action, const int mods)
{
	const KeyBindings& keyBindings = this->game.options.keyBindings;
	bool isPressed = action == GLFW_REPEAT || action == GLFW_PRESS;
	bool result = false;

	if (key == keyBindings.contractFields)
	{
		this->doContractFields = isPressed;
		result = true;
	}
	else if (key == keyBindings.expandFields)
	{
		this->doExpandFields = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToBack)
	{
		this->doPanToBack = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToBottom)
	{
		this->doPanToBottom = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToFront)
	{
		this->doPanToFront = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToLeft)
	{
		this->doPanToLeft = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToPerspective)
	{
		this->doPanToPerspective = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToRight)
	{
		this->doPanToRight = isPressed;
		result = true;
	}
	else if (key == keyBindings.panToTop)
	{
		this->doPanToTop = isPressed;
		result = true;
	}
	else if (key == keyBindings.pitchDown)
	{
		this->doPitchDown = isPressed;
		result = true;
	}
	else if (key == keyBindings.pitchUp)
	{
		this->doPitchUp = isPressed;
		result = true;
	}
	else if (key == keyBindings.rollClockwise)
	{
		this->doRollClockwise = isPressed;
		result = true;
	}
	else if (key == keyBindings.rollCounterClockwise)
	{
		this->doRollCounterClockwise = isPressed;
		result = true;
	}
	else if (key == keyBindings.yawLeft)
	{
		this->doYawLeft = isPressed;
		result = true;
	}
	else if (key == keyBindings.yawRight)
	{
		this->doYawRight = isPressed;
		result = true;
	}
	else if (key == keyBindings.zoomIn)
	{
		this->doZoomIn = isPressed;
		result = true;
	}
	else if (key == keyBindings.zoomOut)
	{
		this->doZoomOut = isPressed;
		result = true;
	}

	return result;
}

bool Scene::handleMouseClickEvent(const int button, const int action)
{
	bool result = false;

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			this->startDragging(button);
		}
		else if (action == GLFW_RELEASE)
		{
			if (!this->isDraggingCamera)
			{
				result = this->revealSelectedField();
			}
			this->stopDragging(button);
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			this->startDragging(button);
		}
		else if (action == GLFW_RELEASE)
		{
			if (!this->isDraggingCamera)
			{
				result = this->markSelectedField();
			}
			this->stopDragging(button);
		}
	}

	return result;
}

bool Scene::handleMouseMovementEvent(const float x, const float y)
{
	bool result = true;

	if (this->game.options.dragButton != DragButton::none && this->dragMouseButton != -1 && !this->isDraggingCamera)
	{
		float deltaX = abs(this->dragLastX - x);
		float deltaY = abs(this->dragLastY - y);
		if (deltaX > this->game.options.dragThreshold || deltaY > this->game.options.dragThreshold)
		{
			this->isDraggingCamera = true;
			this->isMouseActive = false;
			this->clearSelection();
			this->dragLastX = x;
			this->dragLastY = y;
		}
	}

	if (this->isDraggingCamera)
	{
		float deltaX = this->dragLastX - x;
		float deltaY = this->dragLastY - y;
		auto sensitivity = (float)this->game.options.dragSensitivity / 5.0f;
		this->camera.yaw(deltaX * sensitivity);
		this->camera.pitch(deltaY * sensitivity);
		this->dragLastX = x;
		this->dragLastY = y;
		this->updateView();
	}
	else
	{
		this->isMouseActive = true;
		this->updateMouseRay();
	}

	return result;
}

bool Scene::handleMouseScrollEvent(const float delta)
{
	this->camera.zoom(-delta * this->camera.maxDistance / 10.0f);
	return true;
}

void Scene::incrementNeighbourBombCount(const int x, const int y, const int z)
{
	if (this->areValidCoordinates(x, y, z))
	{
		Field* field = this->getField(x, y, z);
		++field->neighbourBombCount;
	}
}

void Scene::incrementNeighbourMarkCount(const int x, const int y, const int z, const int delta)
{
	if (this->areValidCoordinates(x, y, z))
	{
		Field* field = this->getField(x, y, z);
		field->neighbourMarkCount += delta;
	}
}

bool Scene::markSelectedField()
{
	bool result = false;

	if (this->game.status == GameStatus::started)
	{
		int index = this->getSelectedFieldIndex();

		if (index >= 0)
		{
			Field& field = this->fields[index];
			if (this->bombsLeft > 0 || field.isMarked)
			{
				int delta;
				int sound;

				if (field.mark())
				{
					delta = 1;
					sound = this->markSound;
				}
				else
				{
					delta = -1;
					sound = this->unmarkSound;
				}

				array<int, 3> coordinates = this->getFieldCoordinates(index);
				this->bombsLeft -= delta;
				this->incrementNeighbourMarkCount(coordinates[0] - 1, coordinates[1], coordinates[2], delta);
				this->incrementNeighbourMarkCount(coordinates[0] + 1, coordinates[1], coordinates[2], delta);
				this->incrementNeighbourMarkCount(coordinates[0], coordinates[1] - 1, coordinates[2], delta);
				this->incrementNeighbourMarkCount(coordinates[0], coordinates[1] + 1, coordinates[2], delta);
				this->incrementNeighbourMarkCount(coordinates[0], coordinates[1], coordinates[2] - 1, delta);
				this->incrementNeighbourMarkCount(coordinates[0], coordinates[1], coordinates[2] + 1, delta);
				if (!this->checkVictory())
				{
					this->game.resourceManager.playSound(sound);
				}
				this->updateMouseRay();
			}
			result = true;
		}
	}

	return result;
}

void Scene::repositionFields()
{
	float xOffset = (this->width - 1) * this->fieldDistance / -2.0f;
	float yOffset = (this->height - 1) * this->fieldDistance / -2.0f;
	float zOffset = (this->depth - 1) * this->fieldDistance / -2.0f;

	for (int i = 0; i < this->width; ++i)
	{
		for (int j = 0; j < this->height; ++j)
		{
			for (int k = 0; k < this->depth; ++k)
			{
				Field* field = this->getField(i, j, k);
				float x = (float)i * this->fieldDistance + xOffset;
				float y = (float)j * this->fieldDistance + yOffset;
				float z = (float)k * this->fieldDistance + zOffset;
				field->setSize(this->fieldSize);
				field->setPosition(x, y, z);
			}
		}
	}
}

void Scene::refresh()
{
	int totalFieldCount = this->getTotalFieldCount();
	for (int i = 0; i < totalFieldCount; ++i)
	{
		this->fields[i].refresh();
	}
}

void Scene::reset()
{
	int totalFieldCount = this->getTotalFieldCount();
	for (int i = 0; i < totalFieldCount; ++i)
	{
		this->fields[i].reset();
	}
	this->bombsLeft = this->bombCount;
	this->repositionFields();
	this->areFieldsBombed = false;
	this->updateView();
}

void Scene::revealField(const int x, const int y, const int z)
{
	if (this->areValidCoordinates(x, y, z))
	{
		Field* field = this->getField(x, y, z);
		if (!field->isRevealed && !field->isMarked)
		{
			field->reveal();
			if (field->neighbourBombCount == 0)
			{
				this->revealField(x - 1, y, z);
				this->revealField(x + 1, y, z);
				this->revealField(x, y - 1, z);
				this->revealField(x, y + 1, z);
				this->revealField(x, y, z - 1);
				this->revealField(x, y, z + 1);
			}
		}
	}
}

void Scene::revealField(const int index)
{
	array<int, 3> coordinates = this->getFieldCoordinates(index);
	this->revealField(coordinates[0], coordinates[1], coordinates[2]);
}

bool Scene::revealSelectedField()
{
	bool result = false;

	if (this->game.status == GameStatus::ready || this->game.status == GameStatus::started)
	{
		int index = this->getSelectedFieldIndex();

		if (index >= 0)
		{
			Field& field = this->fields[index];
			if (!field.isMarked)
			{
				if (!this->areFieldsBombed)
				{
					this->bombFields(index);
					this->areFieldsBombed = true;
					this->game.startGame();
				}

				if (field.isBombed)
				{
					int totalFieldCount = this->getTotalFieldCount();
					for (int i = 0; i < totalFieldCount; ++i)
					{
						this->fields[i].neighbourMarkCount = 0;
						this->fields[i].reveal();
					}
					this->game.finishGame(false);
				}
				else
				{
					this->revealField(index);
					if (!this->checkVictory())
					{
						this->game.resourceManager.playSound(this->revealSound);
					}
				}
				this->updateView();
			}
			result = true;
		}
	}

	return result;
}

void Scene::setSize(const int width, const int height, const int depth, const int bombCount)
{
	this->bombCount = bombCount;
	this->width = width;
	this->height = height;
	this->depth = depth;

	int totalFieldCount = this->getTotalFieldCount();
	this->fields.clear();
	for (int i = 0; i < totalFieldCount; ++i)
	{
		this->fields.push_back(Field(this->game.resourceManager));
	}

	this->updateCameraDistance();
	this->game.panToPerspective();
}

void Scene::startDragging(const int button)
{
	DragButton dragButton = this->game.options.dragButton;
	bool isEnabledButton = (int)dragButton == button || dragButton == DragButton::both;
	if (isEnabledButton && this->dragMouseButton == -1)
	{
		this->dragMouseButton = button;
		this->dragLastX = this->game.mouseX;
		this->dragLastY = this->game.mouseY;
	}
}

void Scene::stopDragging(const int button)
{
	if (button == this->dragMouseButton)
	{
		this->isDraggingCamera = false;
		this->dragMouseButton = -1;
		this->handleMouseMovementEvent(this->game.mouseX, this->game.mouseY);
	}
}

void Scene::update(const double deltaTime)
{
	if (this->doReset)
	{
		this->game.newGame();
		this->doReset = false;
	}

	if (this->doUpdateSelection)
	{
		int totalFieldCount = this->getTotalFieldCount();
		int lastSelectedIndex = this->selectedFieldIndex;

		this->selectedFieldIndex = -1;
		for (int i = 0; i < totalFieldCount; ++i)
		{
			this->fields[i].select(false);
		}

		if (this->isMouseActive)
		{
			int selectedIndex = this->getSelectedFieldIndex();
			this->selectedFieldIndex = selectedIndex;
			if (selectedIndex >= 0)
			{
				this->fields[selectedIndex].select(true);
			}
		}

		this->doUpdateSelection = false;
	}

	if (this->doContractFields || this->doExpandFields)
	{
		static float fieldDistanceDelta = 0.0f;
		float maxFieldDistance = (float)this->fieldSize * 2.5f;
		float acceleration = maxFieldDistance / 2.0f;
		float maxDelta = maxFieldDistance / 2.0f;

		fieldDistanceDelta = this->calculateDelta(this->doExpandFields, this->doContractFields, fieldDistanceDelta,
			deltaTime, acceleration, maxDelta);
		if (abs(fieldDistanceDelta) > 0.0f)
		{
			this->fieldDistance += fieldDistanceDelta;
			if (this->fieldDistance > maxFieldDistance)
			{
				this->fieldDistance = maxFieldDistance;
			}
			else if (this->fieldDistance < this->fieldSize)
			{
				this->fieldDistance = this->fieldSize;
			}
			this->repositionFields();
			this->updateCameraDistance();
			this->updateView();
		}
	}

	if (this->doPanToFront)
	{
		this->camera.panToFront();
		this->updateView();
	}
	else if (this->doPanToRight)
	{
		this->camera.panToRight();
		this->updateView();
	}
	else if (this->doPanToBack)
	{
		this->camera.panToBack();
		this->updateView();
	}
	else if (this->doPanToLeft)
	{
		this->camera.panToLeft();
		this->updateView();
	}
	else if (this->doPanToTop)
	{
		this->camera.panToTop();
		this->updateView();
	}
	else if (this->doPanToBottom)
	{
		this->camera.panToBottom();
		this->updateView();
	}
	else if (this->doPanToPerspective)
	{
		this->game.panToPerspective();
	}
	else
	{
		float acceleration = 1440.0f;
		float maxDelta = 360.0f;

		if (this->doPitchDown || this->doPitchUp)
		{
			static float pitchDelta = 0.0f;
			pitchDelta = this->calculateDelta(this->doPitchDown, this->doPitchUp, pitchDelta, deltaTime, acceleration,
				maxDelta);
			if (abs(pitchDelta) > 0.0f)
			{
				this->camera.pitch(pitchDelta * (float)deltaTime);
				this->updateView();
			}
		}

		if (this->doRollClockwise || this->doRollCounterClockwise)
		{
			static float rollDelta = 0.0f;
			rollDelta = this->calculateDelta(this->doRollCounterClockwise, this->doRollClockwise, rollDelta, deltaTime,
				acceleration, maxDelta);
			if (abs(rollDelta) > 0.0f)
			{
				this->camera.roll(rollDelta * (float)deltaTime);
				this->updateView();
			}
		}

		if (this->doYawLeft || this->doYawRight)
		{
			static float yawDelta = 0.0f;
			yawDelta = this->calculateDelta(this->doYawRight, this->doYawLeft, yawDelta, deltaTime, acceleration,
				maxDelta);
			if (abs(yawDelta) > 0.0f)
			{
				this->camera.yaw(yawDelta * (float)deltaTime);
				this->updateView();
			}
		}

		if (this->doZoomIn || this->doZoomOut)
		{
			static float zoomingDelta = 0.0f;
			acceleration = this->fieldSize * 1440.0f;
			maxDelta = this->fieldSize * 360.0f;
			zoomingDelta = this->calculateDelta(this->doZoomOut, this->doZoomIn, zoomingDelta, deltaTime,
				acceleration / 16.0f, maxDelta / 4.0f);
			if (abs(zoomingDelta))
			{
				this->camera.zoom(zoomingDelta * (float)deltaTime);
				this->updateView();
			}
		}
	}
}

void Scene::updateCameraDistance()
{
	int maxEdgeSize = max({ this->width, this->height, this->depth });
	float minDistance = maxEdgeSize * Algorithms::cubeSpaceDiagonal(this->fieldDistance);
	this->camera.setMaxDistance(minDistance * 2.0f);
	this->camera.setMinDistance(minDistance);
}

void Scene::updateMouseRay()
{
	this->doUpdateMouseRay = true;
}

void Scene::updateView()
{
	int totalFieldCount = this->getTotalFieldCount();
	for (int i = 0; i < totalFieldCount; ++i)
	{
		this->fields[i].doUpdateBillboard = true;
	}
	this->updateMouseRay();
}
