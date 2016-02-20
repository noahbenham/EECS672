// Umbrella.h

#ifndef UMBRELLA
#define UMBRELLA

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Cylinder.h"

class Umbrella : public ModelViewWithPhongLighting
{
public:
	Umbrella(float xMin, float yMin, float zMin, float radius, float lenY);
	virtual ~Umbrella();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineUmbrella(float xMin, float yMin, float zMin, float radius, float lenY);
	void renderUmbrella();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Cylinder* base;
	Cylinder* canopy;
};

#endif
