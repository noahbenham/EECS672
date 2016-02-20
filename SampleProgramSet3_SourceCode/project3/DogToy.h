// DogToy.h

#ifndef DOGTOY
#define DOGTOY

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Cylinder.h"
#include "Sphere.h"

class DogToy : public ModelViewWithPhongLighting {
public:
	DogToy(float xMin, float yMin, float zMin, float len, float rad);
	virtual ~DogToy();

	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineDogToy(float xMin, float yMin, float zMin, float len, float rad);
	void renderDogToy();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Cylinder* shaft;
	Sphere* leftSide;
	Sphere* rightSide;
};

#endif
