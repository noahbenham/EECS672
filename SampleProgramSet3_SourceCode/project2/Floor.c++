// Floor.c++

#include "Floor.h"
#include "ShaderIF.h"

Floor::Floor(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	defineFloor(xMin, yMin, zMin, lenX, lenY, lenZ);
}

Floor::~Floor() {
	delete base;
}

void Floor::defineFloor(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	base = new Block(xMin, yMin, zMin, lenX, 0.0, lenZ);

	xmin = xMin;
	xmax = xMin + lenX;
	ymin = yMin;
	ymax = yMin + lenY;
	zmin = zMin;
	zmax = zMin + lenZ;
}

void Floor::renderFloor() {
	float riverBankGreen[] = { 0.0, 0.462, 0.039 };

	base->Block::render();
	base->Block::setColor(riverBankGreen[0], riverBankGreen[1], riverBankGreen[2]);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Floor::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Floor::render() {
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
	renderFloor();

	glUseProgram(pgm);
}
