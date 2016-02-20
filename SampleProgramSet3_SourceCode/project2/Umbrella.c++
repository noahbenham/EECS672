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
	base = new Cylinder(xMin, yMin, zMin, 0.0, 0.0, 90.0, radius * .05, lenY * .95, 1.0);
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

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
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
