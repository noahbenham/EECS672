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
	base = new Block(xMin, yMin, zMin, lenX, 0.01, lenZ);

	xmin = xMin;
	xmax = xMin + lenX;
	ymin = yMin;
	ymax = yMin + lenY;
	zmin = zMin;
	zmax = zMin + lenZ;
}

void Floor::renderFloor() {
	float meadowGreen[] = { 0.0, 0.3608, 0.0353 };

	base->Block::render();
	base->Block::setColor(meadowGreen[0], meadowGreen[1], meadowGreen[2]);
}

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

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	float ka[] =   {0.1, 0.1, 0.1};
	float kd[] =   {0.3, 0.3, 0.3};
	float ks[] =   {0.0, 0.0, 0.0};
	float m = 5;
	glUniform3fv(ppuLoc_ka, 1, ka);
	glUniform3fv(ppuLoc_kd, 1, kd);
	glUniform3fv(ppuLoc_ks, 1, ks);
	glUniform1f(ppuLoc_m, m);

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
