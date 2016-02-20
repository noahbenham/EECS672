// Sphere.h

#ifndef SPHERE
#define SPHERE

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

typedef float vec3[3];

#define SphNumHorizontalPts 50
#define SphNumVerticalPts 50
#define SphTotalPts SphNumHorizontalPts * 2 * (SphNumVerticalPts + 1) + 1

class Sphere : public ModelViewWithPhongLighting {
public:
	Sphere(float xPos, float yPos, float zPos, float rad);
	virtual ~Sphere();

	void getMCBoundingBox(double* xyzLimits) const;
	void render();
	void setColor(float r, float g, float b);
private:
	GLuint vao;
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	float bColor[3];

	void defineSphere(float xPos, float yPos, float zPos, float rad);
	void renderSphere();
};

#endif
