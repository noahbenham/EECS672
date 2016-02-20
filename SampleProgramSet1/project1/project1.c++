// project1.c++
// Noah Benham
// Project 1

#include "GLFWController.h"
#include "ModelView.h"

#include <fstream>

int main(int argc, char* argv[])
{
	GLFWController c(argv[0]);
	c.reportVersions(std::cout);

	// *** CSV Reading Section ***
	// Hard code filename for now
	std::ifstream file;
	if (argc == 1)
		file.open("ExchangeRates.csv");
	else
		file.open(argv[1]);

	// Initial pass through file
	std::string date;
	float rate1, rate2, rate3, rate4, yMax = 0.0;
	char comma;
	int nLines = 0;
	while (file >> date >> rate1 >> comma >> rate2 >> comma >> rate3 >> comma >> rate4) {
		nLines++;
		if (rate1 > yMax)
			yMax = rate1;
		if (rate2 > yMax)
			yMax = rate2;
		if (rate3 > yMax)
			yMax = rate3;
		if (rate4 > yMax)
			yMax = rate4;
	}

	// Close and reopen stream; reset to beginning
	file.close();
	if (argc == 1)
		file.open("ExchangeRates.csv");
	else
		file.open(argv[1]);

	// Setup vertex arrays
	vec2 posVert1[nLines];
	vec2 posVert2[nLines];
	vec2 posVert3[nLines];
	vec2 posVert4[nLines];

	int currPosInFile = 0;
	while (file >> date >> rate1 >> comma >> rate2 >> comma >> rate3 >> comma >> rate4) {
		posVert1[currPosInFile][0] = double(currPosInFile);
		posVert1[currPosInFile][1] = rate1;
		posVert2[currPosInFile][0] = double(currPosInFile);
		posVert2[currPosInFile][1] = rate2;
		posVert3[currPosInFile][0] = double(currPosInFile);
		posVert3[currPosInFile][1] = rate3;
		posVert4[currPosInFile][0] = double(currPosInFile);
		posVert4[currPosInFile][1] = rate4;

		currPosInFile++;
	}

	// Setup axis, horizontal scale lines
	vec2 axis[] = { { nLines, yMax         }, { 0.0, yMax         }, { 0.0, yMax * 0.875 },
			{ nLines, yMax * 0.875 }, { 0.0, yMax * 0.875 }, { 0.0, yMax * 0.750 },
			{ nLines, yMax * 0.750 }, { 0.0, yMax * 0.750 }, { 0.0, yMax * 0.625 },
			{ nLines, yMax * 0.625 }, { 0.0, yMax * 0.625 }, { 0.0, yMax * 0.500 },
			{ nLines, yMax * 0.500 }, { 0.0, yMax * 0.500 }, { 0.0, yMax * 0.375 },
			{ nLines, yMax * 0.375 }, { 0.0, yMax * 0.375 }, { 0.0, yMax * 0.250 },
			{ nLines, yMax * 0.250 }, { 0.0, yMax * 0.250 }, { 0.0, yMax * 0.125 },
			{ nLines, yMax * 0.125 }, { 0.0, yMax * 0.125 }, { 0.0,        0.000 },
			{ nLines,        0.000 }};

	c.addModel( new ModelView(posVert1, 2, nLines) );
	c.addModel( new ModelView(posVert2, 3, nLines) );
	c.addModel( new ModelView(posVert3, 4, nLines) );
	c.addModel( new ModelView(posVert4, 5, nLines) );
	c.addModel( new ModelView(axis, 1, 25) );

	glClearColor(1.0, 1.0, 1.0, 1.0);

	// initialize 2D viewing information...
	// Get overall scene bounding box in Model Coordinates:
	double xyz[6]; // xyz limits, even though this is 2D
	c.getOverallMCBoundingBox(xyz);

	xyz[0] -= xyz[1] * .05;
	xyz[2] -= xyz[3] * .05;

	// Tell ModelView we want to see it all:
	ModelView::setMCRegionOfInterest(xyz);

	c.run();

	return 0;
}
