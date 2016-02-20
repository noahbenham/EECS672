// Glass.h

#ifndef GLASS
#define GLASS

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Cylinder.h"

class Glass : public ModelViewWithPhongLighting
{
public:
	Glass(float xMin, float yMin, float zMin, float radius);
	virtual ~Glass();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineGlass(float xMin, float yMin, float zMin, float radius);
	void renderGlass();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Cylinder* glass;
};

#endif
