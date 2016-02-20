// Custom subclass for Controller

#include "Controller_Additions.h"
#include "ModelView.h"

int xVar, yVar;
bool isClicked;

Controller_Additions::Controller_Additions(const std::string& windowTitle, int rcFlags) : GLFWController(windowTitle, rcFlags) { }

void Controller_Additions::handleMouseButton(MouseButton button, bool pressed, int x, int y, int mods) {
	isClicked = false;
	if (button == LEFT_BUTTON && pressed)
		isClicked = true;

	xVar = x;
	yVar = y;
}

void Controller_Additions::handleMouseMotion(int x, int y) {
	float deltaX = xVar - x;
	float deltaY = yVar - y;

	xVar = x;
	yVar = y;

	if (isClicked == true)
		ModelView::addToGlobalRotationDegrees(deltaX, deltaY, 0.0);

	redraw();
}

void Controller_Additions::handleScroll(bool up) {
	if(up)
		ModelView::addToGlobalZoom(-0.1);
	else
		ModelView::addToGlobalZoom(0.1);

 	redraw();
}
