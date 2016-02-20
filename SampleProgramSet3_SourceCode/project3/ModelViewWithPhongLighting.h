// ModelViewWithPhongLighting.h

#ifndef MODELVIEWWITHPHONGLIGHTING_H
#define MODELVIEWWITHPHONGLIGHTING_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "ShaderIF.h"
#include "Matrix4x4.h"

// Custom int values for perspective type, used in shader
#define C_PERSPECTIVE 0
#define C_ORTHOGONAL 1
#define C_OBLIQUE 2

class ModelViewWithPhongLighting : public ModelView {
public:
	ModelViewWithPhongLighting();
	virtual ~ModelViewWithPhongLighting();

	static void setShaderSources(const std::string& vShader, const std::string& fShader);
	void handleCommand(unsigned char key, double ldsX, double ldsY);

protected:

	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_mcNormal;
	static GLint ppuLoc_mc_ec, ppuLoc_ec_lds;

	static GLint ppuLoc_projType, ppuLoc_numLights;
	static GLint ppuLoc_lightPosition, ppuLoc_lightStrength, ppuLoc_globalAmbient;
	static GLint ppuLoc_m, ppuLoc_ka, ppuLoc_kd, ppuLoc_ks;

	static void fetchGLSLVariableLocations();
	static void establishLights();

private:
	static std::string vShaderSource, fShaderSource;

	void adjustCenter(float xDiff, float yDiff, float zDiff);
};

#endif
