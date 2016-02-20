// Napkin.h

#ifndef NAPKIN
#define NAPKIN

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Block.h"

class Napkin : public ModelViewWithPhongLighting
{
public:
	Napkin(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	virtual ~Napkin();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineNapkin(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	void renderNapkin();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Block* napkinObj;
};

#endif
