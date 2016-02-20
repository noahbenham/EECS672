// Chair.c++

#include "Chair.h"
#include "ShaderIF.h"

Chair::Chair(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	defineChair(xMin, yMin, zMin, lenX, lenY, lenZ);
}

Chair::~Chair() {
	delete base;
	delete firstLeg;
	delete secondLeg;
	delete thirdLeg;
	delete fourthLeg;
	delete backSupportLeft;
	delete backSupportRight;
	delete backSupportRung1;
	delete backSupportRung2;
	delete backSupportRung3;
	delete backSupportRung4;
}

void Chair::defineChair(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	base = new Block(xMin + 0.0, yMin + lenY, zMin + 0.0, lenX * 1.0, 0.1, lenZ * 1.0);
	firstLeg = new Block(xMin + 0.0, yMin + 0.0, zMin + 0.0, .08, lenY * 1.0, .08);
	secondLeg = new Block(xMin + lenX - .08, yMin + 0.0, zMin + 0.0, 0.08, lenY * 1.0, .08);
	thirdLeg = new Block(xMin + 0.0, yMin + 0.0, zMin + lenZ - .08, 0.08, lenY * 1.0, .08);
	fourthLeg = new Block(xMin + lenX - .08, yMin + 0.0, zMin + lenZ - .08, 0.08, lenY * 1.0, .08);
	backSupportLeft = new Block(xMin + 0.0, yMin + lenY, zMin + 0.0, .08, lenY + .5, .08);
	backSupportRight = new Block(xMin + lenX - .08, yMin + lenY, zMin + 0.0, .08, lenY + .5, .08);
	backSupportRung1 = new Block(xMin + 0.0, yMin + lenY * 2.0 + .50, zMin + 0.0, lenX * 1.0, .08, .08);
	backSupportRung2 = new Block(xMin + 0.0, yMin + lenY * 2.0 + .33, zMin + 0.0, lenX * 1.0, .08, .08);
	backSupportRung3 = new Block(xMin + 0.0, yMin + lenY * 2.0 + .16, zMin + 0.0, lenX * 1.0, .08, .08);
	backSupportRung4 = new Block(xMin + 0.0, yMin + lenY * 2.0      , zMin + 0.0, lenX * 1.0, .08, .08);

	xmin = xMin;
	xmax = xMin + lenX;
	ymin = yMin;
	ymax = yMin + lenY * 2.0 + .5;
	zmin = zMin;
	zmax = zMin + lenZ;
}

void Chair::renderChair() {
	float bColor[] = { .8039, 0.5216, 0.2471 };

	base->Block::render();
	base->Block::setColor(bColor[0], bColor[1], bColor[2]);
	firstLeg->Block::render();
	firstLeg->Block::setColor(bColor[0], bColor[1], bColor[2]);
	secondLeg->Block::render();
	secondLeg->Block::setColor(bColor[0], bColor[1], bColor[2]);
	thirdLeg->Block::render();
	thirdLeg->Block::setColor(bColor[0], bColor[1], bColor[2]);
	fourthLeg->Block::render();
	fourthLeg->Block::setColor(bColor[0], bColor[1], bColor[2]);
	backSupportLeft->Block::render();
	backSupportLeft->Block::setColor(bColor[0], bColor[1], bColor[2]);
	backSupportRight->Block::render();
	backSupportRight->Block::setColor(bColor[0], bColor[1], bColor[2]);
	backSupportRung1->Block::render();
	backSupportRung1->Block::setColor(bColor[0], bColor[1], bColor[2]);
	backSupportRung2->Block::render();
	backSupportRung2->Block::setColor(bColor[0], bColor[1], bColor[2]);
	backSupportRung3->Block::render();
	backSupportRung3->Block::setColor(bColor[0], bColor[1], bColor[2]);
	backSupportRung4->Block::render();
	backSupportRung4->Block::setColor(bColor[0], bColor[1], bColor[2]);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Chair::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Chair::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	establishLights();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderChair();

	glUseProgram(pgm);
}
