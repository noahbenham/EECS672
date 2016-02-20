// HalfSphere.h

#ifndef HALFSPHERE
#define HALFSPHERE

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"

typedef float vec3[3];

#define HalfSphNumHorizontalPts 20
#define HalfSphNumVerticalPts 20
#define HalfSphTotalPts HalfSphNumHorizontalPts * 2 * (HalfSphNumVerticalPts + 1) + 1

class HalfSphere : public ModelViewWithPhongLighting {
public:
	HalfSphere(float xPos, float yPos, float zPos, float rad);
	virtual ~HalfSphere();

	void getMCBoundingBox(double* xyzLimits) const;
	void render();
	void setColor(float r, float g, float b);
private:
	GLuint vao;
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	float bColor[3];

	void defineHalfSphere(float xPos, float yPos, float zPos, float rad);
	void renderHalfSphere();
};

#endif
