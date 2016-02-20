// Bowl.h

#ifndef BOWL
#define BOWL

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "HalfSphere.h"

class Bowl : public ModelViewWithPhongLighting {
public:
	Bowl(float xMin, float yMin, float zMin, float rad);
	virtual ~Bowl();

	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineBowl(float xMin, float yMin, float zMin, float rad);
	void renderBowl();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	HalfSphere* BowlObj;
};

#endif
