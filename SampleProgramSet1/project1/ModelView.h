// ModelView.h - a basic combined Model and View for OpenGL
// Noah Benham
// Project 1

#ifndef MODELVIEW_H
#define MODELVIEW_H

class ShaderIF;

#include <string>

#ifdef __APPLE_CC__
#define GLFW_INCLUDE_GLCOREARB
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

typedef float vec2[2];
typedef float vec3[3];

class ModelView
{
public:
	ModelView(vec2* coords, int color, int nVertices);
	virtual ~ModelView();

	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render() const;

	static void setMCRegionOfInterest(double xyz[6]);

private:
	// VAO(s), VBO(s), and other relevant INSTANCE variables
	GLuint vao[1];
	GLuint vbo[13]; // x-axis, y-axis, USD, AUD, BRL, CZK, 8 visual lines
	float vertexColor;
	int numVertices;
	float xmin, xmax, ymin, ymax;

	// assume shared shader program
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	
	static GLint pvaLoc_mcPosition;
	static GLint ppuLoc_vertexColor;
	static GLint ppuLoc_scaleTrans;

	static GLint ppUniformLocation(GLuint glslProgram, const std::string& name);
	static GLint pvAttribLocation(GLuint glslProgram, const std::string& name);

	static void linearMap(double fromMin, double fromMax,
		double toMin, double toMax, double& scale, double& trans);

	static void matchAspectRatio(double& xmin, double& xmax,
		double& ymin, double& ymax, double vAR);

	static void compute2DScaleTrans(float* scaleTrans);

	static double mcRegionOfInterest[6];

	void initModelGeometry(vec2* coords);

	static void fetchGLSLVariableLocations();
};

#endif
