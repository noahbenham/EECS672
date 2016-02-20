// DogToy.c++

#include "DogToy.h"
#include "ShaderIF.h"

DogToy::DogToy(float xMin, float yMin, float zMin, float len, float rad) {
	defineDogToy(xMin, yMin, zMin, len, rad);
}

DogToy::~DogToy() {
	delete shaft;
	delete leftSide;
	delete rightSide;
}

void DogToy::defineDogToy(float xMin, float yMin, float zMin, float len, float rad) {
	// xPos, yPos, zPos, xRot, yRot, zRot, rad, height, adj
	shaft = new Cylinder(xMin + rad * .5, yMin + rad, zMin + rad, 0.0, 0.0, 0.0, rad * .5, len + rad, 1.0);
	leftSide = new Sphere(xMin + rad, yMin + rad, zMin + rad, rad);
	rightSide = new Sphere(xMin + rad + len, yMin + rad, zMin + rad, rad);

	xmin = xMin;
	xmax = xMin + rad * 2;
	ymin = yMin;
	ymax = yMin + rad * 2;
	zmin = zMin;
	zmax = zMin + rad * 2;
}

void DogToy::renderDogToy() {
	float kongRed[] = { 0.8275, 0.1255, 0.2431 };

	shaft->Cylinder::render();
	shaft->Cylinder::setColor(kongRed[0], kongRed[1], kongRed[2]);
	leftSide->Sphere::render();
	leftSide->Sphere::setColor(kongRed[0], kongRed[1], kongRed[2]);
	rightSide->Sphere::render();
	rightSide->Sphere::setColor(kongRed[0], kongRed[1], kongRed[2]);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void DogToy::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void DogToy::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	// Emulate plastic reflectivity
	float ka[] =   {0.0, 0.0, 0.0};
	float kd[] =   {0.01, 0.01, 0.01};
	float ks[] =   {0.5, 0.5, 0.5};
	float m = 32;
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
	renderDogToy();

	glUseProgram(pgm);
}
