// HalfSphere.c++

#include "HalfSphere.h"

HalfSphere::HalfSphere(float xPos, float yPos, float zPos, float rad) {
	bColor[0] = 0.0; //red
	bColor[1] = 0.0; //green
	bColor[2] = 0.0; //blue

	defineHalfSphere(xPos, yPos, zPos, rad);
}

HalfSphere::~HalfSphere() {
	glDeleteBuffers(2, &vbo[0]);
	glDeleteVertexArrays(1, &vao);
}

void HalfSphere::defineHalfSphere(float xPos, float yPos, float zPos, float rad) {
	vec3* coordArr = new vec3[HalfSphTotalPts];
	vec3* normalArr = new vec3[HalfSphTotalPts];

	double th = 0.0;
	double deltaTh = 2.0 * M_PI/(HalfSphNumHorizontalPts - 1);
	double phiVal = M_PI;
	double deltaPhi = M_PI / HalfSphNumVerticalPts;

	for (int currVertPt = 0; currVertPt <= HalfSphNumVerticalPts; currVertPt++) {
		th = 0; // reset theta for every iteration
		double currStripYmin = rad * cos(phiVal); // y axis position
		double currStripYmax = rad * cos(phiVal + deltaPhi);

		for (int currHorizPt = 0; currHorizPt <= HalfSphNumHorizontalPts; currHorizPt++) {
			int horizBufferPt = 2 * currHorizPt;
			double currStripXmin = rad * sin(phiVal) * cos(th);
			double currStripZmin = rad * sin(phiVal) * sin(th);
			double currStripXmax = rad * sin(phiVal + deltaPhi) * cos(th);
			double currStripZmax = rad * sin(phiVal + deltaPhi) * sin(th);

			int currIndex = currVertPt * 2 * HalfSphNumHorizontalPts + horizBufferPt;

			normalArr[currIndex][0] = currStripXmin;
			normalArr[currIndex][1] = currStripYmin;
			normalArr[currIndex][2] = currStripZmin;
			coordArr[currIndex][0] = currStripXmin + xPos;
			coordArr[currIndex][1] = currStripYmin + yPos;
			coordArr[currIndex][2] = currStripZmin + zPos;

			normalArr[currIndex+1][0] = currStripXmax;
			normalArr[currIndex+1][1] = currStripYmax;
			normalArr[currIndex+1][2] = currStripZmax;
			coordArr[currIndex+1][0] = currStripXmax + xPos;
			coordArr[currIndex+1][1] = currStripYmax + yPos;
			coordArr[currIndex+1][2] = currStripZmax + zPos;

			th += deltaTh;
		}
		phiVal += deltaPhi;
	}


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(2, &vbo[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * HalfSphTotalPts, coordArr, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * HalfSphTotalPts, normalArr, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcNormal);

	delete[] coordArr;
	delete[] normalArr;
}

void HalfSphere::getMCBoundingBox(double* xyzLimits) const {
	xyzLimits[0] = -1.0;
	xyzLimits[1] = 1.0;
	xyzLimits[2] = -1.0;
	xyzLimits[3] = 1.0;
	xyzLimits[4] = -1.0;
	xyzLimits[5] = 1.0;
}

void HalfSphere::renderHalfSphere() {
	glBindVertexArray(vao);
	glUniform3fv(ppuLoc_kd, 1, bColor);

	// Only draw bottom half of the sphere
	for (int currPt = 0; currPt < HalfSphNumVerticalPts; currPt++) {
		glDrawArrays(GL_TRIANGLE_STRIP, currPt*HalfSphNumHorizontalPts, HalfSphNumHorizontalPts+2);
	}
}

void HalfSphere::setColor(float r, float g, float b) {
	bColor[0] = r;
	bColor[1] = g;
	bColor[2] = b;
}

void HalfSphere::render() {
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	if(projType == PERSPECTIVE)
		glUniform1i(ppuLoc_projType, C_PERSPECTIVE);
	if(projType == ORTHOGONAL)
		glUniform1i(ppuLoc_projType, C_ORTHOGONAL);
	if(projType == OBLIQUE)
		glUniform1i(ppuLoc_projType, C_OBLIQUE);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	renderHalfSphere();
	establishLights();

	glUseProgram(pgm);
}
