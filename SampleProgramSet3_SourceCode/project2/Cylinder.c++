// Cylinder.c++

#include "Cylinder.h"

Cylinder::Cylinder(float xPos, float yPos, float zPos, float xRotIn, float yRotIn, float zRotIn, float rad, float len, float adj) : displayCylEdges(false), displayCylFill(true) {
	bColor[0] = 0.0; //red
	bColor[1] = 0.0; //green
	bColor[2] = 0.0; //blue

	defineCylinder(xPos, yPos, zPos, xRotIn, yRotIn, zRotIn, rad, len, adj);
}

Cylinder::~Cylinder() {
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

const int N_POINTS_AROUND_SLICE = 18; // number points around a cross-section

void Cylinder::defineCylinder(float xPos, float yPos, float zPos, float xRotIn, float yRotIn, float zRotIn, float rad, float len, float adj) {
	typedef float vec3[3];
	int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];

	cryph::Matrix4x4 tran = cryph::Matrix4x4::translation(cryph::AffVector(xPos, yPos, zPos));
	cryph::Matrix4x4 xRot = cryph::Matrix4x4::xRotationDegrees(xRotIn);
	cryph::Matrix4x4 yRot = cryph::Matrix4x4::yRotationDegrees(yRotIn);
	cryph::Matrix4x4 zRot = cryph::Matrix4x4::zRotationDegrees(zRotIn);
	cryph::Matrix4x4 finalTrans = tran * xRot * yRot * zRot;
	

	double theta = 0.0;
	double dTheta = 2.0 * M_PI/N_POINTS_AROUND_SLICE;
	double yb = 0.0, zb = 0.0;
	double x1 = 0.0, x2 = len;
	double r = rad;

	for (int i=0 ; i<=N_POINTS_AROUND_SLICE ; i++) {
		int j = 2 * i;
		double dy = cos(theta);
		double dz = sin(theta);

		normals[j][0] = 0.0; normals[j][1] = dy; normals[j][2] = dz;
		coords[j][0] = x1; coords[j][1] = yb + r*dy; coords[j][2] = zb + r*dz;
		normals[j+1][0] = 0.0; normals[j+1][1] = dy; normals[j+1][2] = dz;
		coords[j+1][0] = x2; coords[j+1][1] = yb + r*dy*adj; coords[j+1][2] = zb + r*dz*adj;

		theta += dTheta;
	}

	for (int i=0; i < nPoints; i++) {
		(finalTrans * cryph::AffVector(normals[i])).vComponents(normals[i]);
		(finalTrans * cryph::AffPoint(coords[i])).aCoords(coords[i]);
	}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcNormal);

	delete [] coords;
	delete [] normals;
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Cylinder::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = -1.0;
	xyzLimits[1] = 1.0;
	xyzLimits[2] = -1.0;
	xyzLimits[3] = 1.0;
	xyzLimits[4] = -1.0;
	xyzLimits[5] = 1.0;
}

void Cylinder::renderCylinder(float* color)
{
	typedef float vec3[3];
	glUniform3fv(ppuLoc_kd, 1, bColor);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*(N_POINTS_AROUND_SLICE+1));
}

void Cylinder::setColor(float r, float g, float b) {
	trans = 1.0;
	bColor[0] = r;
	bColor[1] = g;
	bColor[2] = b;
}

void Cylinder::setColor(float r, float g, float b, float t) {
	bColor[0] = r;
	bColor[1] = g;
	bColor[2] = b;
	trans = t;
}

void Cylinder::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	renderCylinder(bColor);

	establishLights();

	glUseProgram(pgm);
}
