// Glass.c++

// TODO implement transparency in future iterations

#include "Glass.h"
#include "ShaderIF.h"

Glass::Glass(float xMin, float yMin, float zMin, float radius) {
	defineGlass(xMin, yMin, zMin, radius);
}

Glass::~Glass() {
	delete glass;
}

void Glass::defineGlass(float xMin, float yMin, float zMin, float radius) {
	// xPos, yPos, zPos, xRot, yRot, zRot, rad, height, adj
	glass = new Cylinder(xMin, yMin, zMin, 0.0, 0.0, 90.0, radius, radius * 3.5, 1.0);

	xmin = xMin - radius;
	xmax = xMin + radius;
	ymin = yMin;
	ymax = yMin + radius * 1.5;
	zmin = zMin - radius;
	zmax = zMin + radius;
}

void Glass::renderGlass() {
	float bColor[] = { 0.6627, 0.6627, 0.6627 };
	float transparency = 0.5; // currently inactive

	glass->Cylinder::render();
	glass->Cylinder::setColor(bColor[0], bColor[1], bColor[2], transparency);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Glass::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Glass::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	// Emulate polished silver
	float ka[] =   {0.23125, 0.23125, 0.23125};
	float kd[] =   {0.2775, 0.2775, 0.2775};
	float ks[] =   {0.773911, 0.773911, 0.773911};
	float m = 89.6;
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
	renderGlass();

	glUseProgram(pgm);
}
