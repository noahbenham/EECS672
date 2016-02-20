// Table.c++

#include "Table.h"
#include "ShaderIF.h"

Table::Table(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	defineTable(xMin, yMin, zMin, lenX, lenY, lenZ);
}

Table::~Table() {
	delete base;
	delete firstLeg;
	delete secondLeg;
	delete thirdLeg;
	delete fourthLeg;
}

void Table::defineTable(float xMin, float yMin, float zMin, float lenX, float lenY, float lenZ) {
	base = new Block(xMin + 0.0, yMin + lenY * 1.5, zMin + 0.0, lenX * 1.0, 0.1, lenZ * 1.0);
	firstLeg = new Block(xMin + 0.0, yMin + 0.0, zMin + 0.0, .08, lenY * 1.5, .08);
	secondLeg = new Block(xMin + lenX - .08, yMin + 0.0, zMin + 0.0, 0.08, lenY * 1.5, .08);
	thirdLeg = new Block(xMin + 0.0, yMin + 0.0, zMin + lenZ - .08, 0.08, lenY * 1.5, .08);
	fourthLeg = new Block(xMin + lenX - .08, yMin + 0.0, zMin + lenZ - .08, 0.08, lenY * 1.5, .08);

	xmin = xMin;
	xmax = xMin + lenX;
	ymin = yMin;
	ymax = yMin + lenY * 1.5 + 0.1;
	zmin = zMin;
	zmax = zMin + lenZ;
}

void Table::renderTable() {
	float woodBrown[] = { .8039, 0.5216, 0.2471 };
	float granite[] = { .3176, .2980, .2902 };

	base->Block::render();
	base->Block::setColor(granite[0], granite[1], granite[2]);
	firstLeg->Block::render();
	firstLeg->Block::setColor(woodBrown[0], woodBrown[1], woodBrown[2]);
	secondLeg->Block::render();
	secondLeg->Block::setColor(woodBrown[0], woodBrown[1], woodBrown[2]);
	thirdLeg->Block::render();
	thirdLeg->Block::setColor(woodBrown[0], woodBrown[1], woodBrown[2]);
	fourthLeg->Block::render();
	fourthLeg->Block::setColor(woodBrown[0], woodBrown[1], woodBrown[2]);
}

void Table::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

void Table::render() {
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
	float kd[] =   {0.1, 0.1, 0.1};
	float ks[] =   {0.1, 0.1, 0.1};
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
	renderTable();

	glUseProgram(pgm);
}
