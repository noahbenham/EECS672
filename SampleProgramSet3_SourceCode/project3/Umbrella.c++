// Umbrella.c++

#include "Umbrella.h"
#include "ShaderIF.h"

Umbrella::Umbrella(float xMin, float yMin, float zMin, float radius, float lenY) {
	defineUmbrella(xMin, yMin, zMin, radius, lenY);
}

Umbrella::~Umbrella() {
	delete base;
	delete canopy;
}

void Umbrella::defineUmbrella(float xMin, float yMin, float zMin, float radius, float lenY) {
	// 		    xPos, yPos, zPos, xRot, yRot, zRot, rad, height, adj
	base = new Cylinder(xMin, yMin, zMin, 0.0, 0.0, 90.0, radius * .05, lenY * .99, 1.0);
	canopy = new Cylinder(xMin, lenY - lenY * .2, zMin, 0.0, 0.0, 90.0, radius, lenY * .2, 0.0);

	xmin = xMin - radius;
	xmax = xMin + radius;
	ymin = yMin;
	ymax = yMin + lenY;
	zmin = zMin - radius;
	zmax = zMin + radius;
}

void Umbrella::renderUmbrella() {
	float barkBrown[] = { .384, .306, .173 };
	float firebrick[] = { .698, .133, .133 };

	base->Cylinder::render();
	base->Cylinder::setColor(barkBrown[0], barkBrown[1], barkBrown[2]);
	canopy->Cylinder::render();
	canopy->Cylinder::setColor(firebrick[0], firebrick[1], firebrick[2]);
}

void Umbrella::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Umbrella::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	float ka[] = {0.5, 0.5, 0.5};
	float kd[] = {0.5, 0.5, 0.5};
	float ks[] = {0.1, 0.1, 0.1};
	float m = 8;
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
	renderUmbrella();

	glUseProgram(pgm);
}
