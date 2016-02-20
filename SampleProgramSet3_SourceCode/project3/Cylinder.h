// Cylinder.h

#ifndef CYLINDER
#define CYLINDER

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

class Cylinder : public ModelViewWithPhongLighting {
public:
	Cylinder(float xPos, float yPos, float zPos,
		float xRotIn, float yRotIn, float zRotIn, float rad,
		float len, float adj);
	virtual ~Cylinder();

	void getMCBoundingBox(double* xyzLimits) const;
	void render();
	void setColor(float r, float g, float b);
	void setColor(float r, float g, float b, float t);
private:
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	bool displayCylEdges, displayCylFill;

	float xRot, yRot, zRot;

	float bColor[3];
	float trans;

	void defineCylinder(float xPos, float yPos, float zPos,
		float xRotIn, float yRotIn, float zRotIn, float rad,
		float len, float adj);
	void renderCylinder();
};

#endif
