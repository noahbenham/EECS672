// ModelView.c++ - a basic combined Model and View for OpenGL
// Noah Benham
// Project 1

#include <iostream>

#include "ModelView.h"
#include "Controller.h"
#include "ShaderIF.h"

ShaderIF* ModelView::shaderIF = NULL;
int ModelView::numInstances = 0;
GLuint ModelView::shaderProgram = 0;
GLint ModelView::ppuLoc_vertexColor = -2;
GLint ModelView::pvaLoc_mcPosition = -2;
GLint ModelView::ppuLoc_scaleTrans = -2;

double ModelView::mcRegionOfInterest[6] = { -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 };

ModelView::ModelView(vec2* coords, int color, int nVertices) : numVertices(nVertices)
{
	if (ModelView::shaderProgram == 0)
	{
		// Create the common shader program:
		ModelView::shaderIF = new ShaderIF("project1.vsh", "project1.fsh");
		ModelView::shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}

	// define and call method(s) to initialize your model and send data to GPU
	vertexColor = color;
	initModelGeometry(coords);
	ModelView::numInstances++;
}

ModelView::~ModelView()
{
	// delete the vertex array objects and buffers here
	glDeleteBuffers(13, vbo);
	glDeleteVertexArrays(1, vao);
	if (--numInstances == 0)
	{
		ModelView::shaderIF->destroy();
		delete ModelView::shaderIF;
		ModelView::shaderIF = NULL;
		ModelView::shaderProgram = 0;
	}
}

void ModelView::compute2DScaleTrans(float* scaleTransF) // CLASS METHOD
{
	double xmin = mcRegionOfInterest[0];
	double xmax = mcRegionOfInterest[1];
	double ymin = mcRegionOfInterest[2];
	double ymax = mcRegionOfInterest[3];

	// If we wish to preserve aspect ratios, make "region of interest"
	// wider or taller to match the Controller's viewport aspect ratio.
	// double vAR = Controller::getCurrentController()->getViewportAspectRatio();
	// matchAspectRatio(xmin, xmax, ymin, ymax, vAR);

	// We are only concerned with the xy extents for now, hence we will
	// ignore mcRegionOfInterest[4] and mcRegionOfInterest[5].
	// Map the overall limits to the -1..+1 range expected by the OpenGL engine:
	double scaleTrans[4];
	linearMap(xmin, xmax, -1.0, 1.0, scaleTrans[0], scaleTrans[1]);
	linearMap(ymin, ymax, -1.0, 1.0, scaleTrans[2], scaleTrans[3]);
	for (int i=0 ; i<4 ; i++)
		scaleTransF[i] = static_cast<float>(scaleTrans[i]);
}

void ModelView::fetchGLSLVariableLocations()
{
	if (ModelView::shaderProgram > 0)
	{
		// Set GLSL variable locations
		ModelView::ppuLoc_vertexColor = ppUniformLocation(shaderProgram, "vertexColor");
		ModelView::pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		ModelView::ppuLoc_scaleTrans = ppUniformLocation(shaderProgram, "scaleTrans");
	}
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void ModelView::getMCBoundingBox(double* xyzLimits) const
{
	// Put this ModelView instance's min and max x, y, and z extents
	// into xyzLimits[0..5]. (-1 .. +1 is OK for z direction for 2D models)
	xyzLimits[0] = xmin; xyzLimits[1] = xmax;
	xyzLimits[2] = ymin; xyzLimits[3] = ymax;
	xyzLimits[4] = -1.0; xyzLimits[5] = 1.0; // (zmin, zmax) (really 0..0)
}

void ModelView::handleCommand(unsigned char key, double ldsX, double ldsY)
{
}

void ModelView::initModelGeometry(vec2* coords)
{
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// create three per-vertex attribute buffers for the set of triangles
	// [0] - color buffer; [1] - fraction; [2] - vertex coordinate buffer
	glGenBuffers(3, vbo);

	// Allocate space for and copy CPU coordinate data to GPU
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	int numBytesCoordinateData = numVertices * sizeof(vec2);
	glBufferData(GL_ARRAY_BUFFER, numBytesCoordinateData, coords, GL_STATIC_DRAW);
	glVertexAttribPointer(ModelView::pvaLoc_mcPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(ModelView::pvaLoc_mcPosition);

	// make note of the min/max coordinates
	xmin = xmax = coords[0][0];
	ymin = ymax = coords[0][1];
	for (int i=1 ; i<numVertices ; i++)
	{
		if (coords[i][0] < xmin)
			xmin = coords[i][0];
		else if (coords[i][0] > xmax)
			xmax = coords[i][0];
		if (coords[i][1] < ymin)
			ymin = coords[i][1];
		else if (coords[i][1] > ymax)
			ymax = coords[i][1];
	}
}

// linearMap determines the scale and translate parameters needed in
// order to map a value, f (fromMin <= f <= fromMax) to its corresponding
// value, t (toMin <= t <= toMax). Specifically: t = scale*f + trans.
void ModelView::linearMap(double fromMin, double fromMax, double toMin, double toMax,
					  double& scale, double& trans) // CLASS METHOD
{
	scale = (toMax - toMin) / (fromMax - fromMin);
	trans = toMin - scale*fromMin;
}

void ModelView::matchAspectRatio(double& xmin, double& xmax,
        double& ymin, double& ymax, double vAR)
{
	double wHeight = ymax - ymin;
	double wWidth = xmax - xmin;
	double wAR = wHeight / wWidth;
	if (wAR > vAR)
	{
		// make window wider
		wWidth = wHeight / vAR;
		double xmid = 0.5 * (xmin + xmax);
		xmin = xmid - 0.5*wWidth;
		xmax = xmid + 0.5*wWidth;
	}
	else
	{
		// make window taller
		wHeight = wWidth * vAR;
		double ymid = 0.5 * (ymin + ymax);
		ymin = ymid - 0.5*wHeight;
		ymax = ymid + 0.5*wHeight;
	}
}

GLint ModelView::ppUniformLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetUniformLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-primitive uniform: '" << name << "'\n";
	return loc;
}

GLint ModelView::pvAttribLocation(GLuint glslProgram, const std::string& name)
{
	GLint loc = glGetAttribLocation(glslProgram, name.c_str());
	if (loc < 0)
		std::cerr << "Could not locate per-vertex attribute: '" << name << "'\n";
	return loc;
}

void ModelView::render() const
{
	// save the current GLSL program in use
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);

	// draw the triangles using our vertex and fragment shaders
	glUseProgram(shaderProgram);

	// set scaleTrans (and all other needed) uniform(s)
	float scaleTrans[4];
	compute2DScaleTrans(scaleTrans);
	glUniform4fv(ModelView::ppuLoc_scaleTrans, 1, scaleTrans);
	glUniform1i(ModelView::ppuLoc_vertexColor, vertexColor);

	// make require primitive call(s)
	glBindVertexArray(vao[0]); // reestablishes all buffer settings as noted above
	
	//glUniform1i(ModelView::ppuLoc_colorMode, colorMode);
	glDrawArrays(GL_LINE_STRIP, 0, numVertices);

	// restore the previous program
	glUseProgram(pgm);
}

void ModelView::setMCRegionOfInterest(double xyz[6])
{
	for (int i=0 ; i<6 ; i++)
		mcRegionOfInterest[i] = xyz[i];
}
