// Napkin.c++

#include "Napkin.h"
#include "ShaderIF.h"

Napkin::Napkin(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	defineNapkin(xMin, yMin, zMin, lenX, lenY, lenZ);
}

Napkin::~Napkin() {
	delete napkinObj;
}

void Napkin::defineNapkin(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	napkinObj = new Block(xMin, yMin, zMin, lenX * 0.2, lenY * 0.03, lenZ * 0.7);

	xmin = xMin;
	xmax = xMin + lenX * 0.2;
	ymin = yMin;
	ymax = yMin + lenY * 0.03;
	zmin = zMin;
	zmax = zMin + lenZ * 0.7;
}

void Napkin::renderNapkin() {
	float bColor[] = { 1.0, 1.0, 1.0 };

	napkinObj->Block::render();
	napkinObj->Block::setColor(bColor[0], bColor[1], bColor[2]);
}

void Napkin::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Napkin::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	float ka[] =   {0.4, 0.4, 0.4};
	float kd[] =   {0.4, 0.4, 0.4};
	float ks[] =   {0.0, 0.0, 0.0};
	float m = 2;
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
	renderNapkin();

	glUseProgram(pgm);
}
