// Chair.h

#ifndef CHAIR
#define CHAIR

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Block.h"

class Chair : public ModelViewWithPhongLighting
{
public:
	Chair(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	virtual ~Chair();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineChair(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	void renderChair();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Block* base;
	Block* firstLeg;
	Block* secondLeg;
	Block* thirdLeg;
	Block* fourthLeg;
	Block* backSupportLeft;
	Block* backSupportRight;
	Block* backSupportRung1;
	Block* backSupportRung2;
	Block* backSupportRung3;
	Block* backSupportRung4;
};

#endif
