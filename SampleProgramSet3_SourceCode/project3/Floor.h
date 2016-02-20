// Floor.h

#ifndef FLOOR
#define FLOOR

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Block.h"

class Floor : public ModelViewWithPhongLighting
{
public:
	Floor(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	virtual ~Floor();

	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineFloor(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	void renderFloor();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Block* base;
};

#endif
