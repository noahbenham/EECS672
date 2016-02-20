// Bowl.c++

#include "Bowl.h"
#include "ShaderIF.h"

Bowl::Bowl(float xMin, float yMin, float zMin, float rad) {
	defineBowl(xMin, yMin, zMin, rad);
}

Bowl::~Bowl() {
	delete BowlObj;
}

void Bowl::defineBowl(float xMin, float yMin, float zMin, float rad) {
	BowlObj = new HalfSphere(xMin, yMin + rad, zMin, rad);

	xmin = xMin - rad;
	xmax = xMin + rad;
	ymin = yMin;
	ymax = yMin + (rad * 2);
	zmin = zMin - rad;
	zmax = zMin + rad;
}

void Bowl::renderBowl() {
	float bColor[] = { 0.1, 0.1, 1.0 };
	
	BowlObj->HalfSphere::setColor(bColor[0], bColor[1], bColor[2]);
	BowlObj->HalfSphere::render();
}

void Bowl::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Bowl::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	float ka[] =   {0.3, 0.6, 0.5};
	float kd[] =   {0.9, 0.9, 0.9};
	float ks[] =   {0.9, 0.9, 0.9};
	float m = 20;
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
	renderBowl();

	glUseProgram(pgm);
}
