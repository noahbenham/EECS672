// Block.h

#ifndef BLOCK
#define BLOCK

#include "ModelViewWithPhongLighting.h"

class Block : public ModelViewWithPhongLighting {
public:
	Block(float cx, float cy, float cz, // lower left corner
	      float lx, float ly, float lz);// lengths in 3 directions
	virtual ~Block();

	void getMCBoundingBox(double* xyzLimits) const;
	void render();

	void setColor(float r, float g, float b);

private:
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[3];

	float xmin, xmax, ymin, ymax, zmin, zmax;

	float bColor[3];

	static GLuint indices[3][4];

	void defineBlock();
	void renderBlock(float* color);
};

#endif
