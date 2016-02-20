// Controller_Additions.h

#ifndef CONTROLLER_ADDITIONS
#define CONTROLLER_ADDITIONS

#include <GL/gl.h>

#include "GLFWController.h"

class Controller_Additions : public GLFWController {
public:
	Controller_Additions(const std::string& windowTitle, int rcFlags);
	void handleMouseButton(MouseButton button, bool pressed, int x, int y, int mods);
	void handleMouseMotion(int x, int y);
	void handleScroll(bool up);

private:
	
};

#endif
