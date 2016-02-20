// Table.h

#ifndef TABLE
#define TABLE

#include <GL/gl.h>

#include "ModelViewWithPhongLighting.h"
#include "Block.h"

class Table : public ModelViewWithPhongLighting
{
public:
	Table(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	virtual ~Table();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimitsF) const;
	void render();
private:
	void defineTable(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ);
	void renderTable();

	float xmin, xmax, ymin, ymax, zmin, zmax;

	Block* base;
	Block* firstLeg;
	Block* secondLeg;
	Block* thirdLeg;
	Block* fourthLeg;
};

#endif
