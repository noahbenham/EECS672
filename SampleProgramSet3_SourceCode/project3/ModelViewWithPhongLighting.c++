// ModelViewWithPhongLighting.c++

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithPhongLighting.h"
#include "ShaderIF.h"

ShaderIF* ModelViewWithPhongLighting::shaderIF = NULL;
int ModelViewWithPhongLighting::numInstances = 0;
GLuint ModelViewWithPhongLighting::shaderProgram = 0;
GLint ModelViewWithPhongLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithPhongLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;

GLint ModelViewWithPhongLighting::ppuLoc_projType = -2;
GLint ModelViewWithPhongLighting::ppuLoc_numLights = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightPosition = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightStrength = -2; // Li
GLint ModelViewWithPhongLighting::ppuLoc_globalAmbient = -2; // La
GLint ModelViewWithPhongLighting::ppuLoc_ka = -2; // fraction incident ambient light reflected
GLint ModelViewWithPhongLighting::ppuLoc_kd = -2; // fraction incident light diffusely reflected
GLint ModelViewWithPhongLighting::ppuLoc_ks = -2; // fraction incident light specularly reflected
GLint ModelViewWithPhongLighting::ppuLoc_m = -2; // specular coefficient

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

int numLights = 3;

float lightPosition[] = {-1.0, 0.0, 1.0, 0.0, // source 0: directional
			 0.0, 1.0, 1.0, 0.0,  // source 1: directional
			 8.0, 50.0, 5.0, 1.0}; // source 2: an actual location in the scene

float lightStrength[] = {0.5, 0.5, 0.5, // 50% white
			 0.6, 0.6, 0.6, // 60% white
			 0.8, 0.8, 0.8}; // 80% yellow

float globalAmbient[] = {0.35, 0.35, 0.35};

// The following variables are set within individual object classes
// "ka" - fraction of incident ambient light that is reflected
// "kd" - diffuse reflectivity; basic object color
// "ks" - specular reflection
// "m" - specular coefficient

// previous defaults:
//float ka[] =   {0.3, 0.6, 0.5};
//float kd[] =   {0.9, 0.9, 0.9};
//float ks[] =   {0.9, 0.9, 0.9};
//float m = 10;


ModelViewWithPhongLighting::ModelViewWithPhongLighting() {
	if (shaderProgram == 0)	{
		shaderIF = new ShaderIF(vShaderSource, fShaderSource);
		shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
	numInstances++;
}

ModelViewWithPhongLighting::~ModelViewWithPhongLighting() {
	if (--numInstances == 0) {
		shaderIF->destroy();
		delete shaderIF;
		shaderIF = NULL;
		shaderProgram = 0;
	}
}

void ModelViewWithPhongLighting::fetchGLSLVariableLocations() {
	if (shaderProgram > 0) {
		pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		pvaLoc_mcNormal = pvAttribLocation(shaderProgram, "mcNormal");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");

		ppuLoc_projType = ppUniformLocation(shaderProgram, "projType");
		ppuLoc_numLights = ppUniformLocation(shaderProgram, "numLights");
		ppuLoc_lightPosition = ppUniformLocation(shaderProgram, "lightPosition");
		ppuLoc_lightStrength = ppUniformLocation(shaderProgram, "lightStrength");
		ppuLoc_globalAmbient = ppUniformLocation(shaderProgram, "globalAmbient");

		ppuLoc_ka = ppUniformLocation(shaderProgram, "ka");
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_ks = ppUniformLocation(shaderProgram, "ks");
		ppuLoc_m = ppUniformLocation(shaderProgram, "m");
	}
}

void ModelViewWithPhongLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc) {
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}

void ModelViewWithPhongLighting::establishLights() {
	glUniform1i(ppuLoc_projType, C_PERSPECTIVE); // match initial view set in project3.cpp
	glUniform1i(ppuLoc_numLights, numLights);
	glUniform4fv(ppuLoc_lightPosition, numLights, lightPosition);
	glUniform3fv(ppuLoc_lightStrength, numLights, lightStrength);
	glUniform3fv(ppuLoc_globalAmbient, 1, globalAmbient);

	/*float ka[] =   {0.3, 0.6, 0.5};
	float kd[] =   {0.9, 0.9, 0.9};
	float ks[] =   {0.9, 0.9, 0.9};
	float m = 10;
	glUniform3fv(ppuLoc_ka, 1, ka);
	glUniform3fv(ppuLoc_kd, 1, kd);
	glUniform3fv(ppuLoc_ks, 1, ks);
	glUniform1f(ppuLoc_m, m);*/
}

void ModelViewWithPhongLighting::adjustCenter(float xDiff, float yDiff, float zDiff) {
	cryph::Matrix4x4 trans = cryph::Matrix4x4::translation(cryph::AffVector(xDiff, yDiff, zDiff));
	dynamic_view = trans * dynamic_view;
}

void ModelViewWithPhongLighting::handleCommand(unsigned char key, double ldsX, double ldsY) {
	// glUniform1i for ppuLoc_projType is set in individual object render methods

	float adjAmount = 0.05;

	if (key=='p')
		projType = PERSPECTIVE;
	else if (key=='o')
		projType = ORTHOGONAL;
	else if (key=='q')
		projType = OBLIQUE;
	/*else if (key=='a') // move eye -x
		adjustCenter(adjAmount, 0.0, 0.0);
	else if (key=='s') // move eye -y
		adjustCenter(0.0, adjAmount, 0.0);
	else if (key=='d') // move eye +x
		adjustCenter(-adjAmount, 0.0, 0.0);
	else if (key=='w') // move eye +y
		adjustCenter(0.0, -adjAmount, 0.0);
	else if (key=='r') // move eye -z
		adjustCenter(0.0, 0.0, adjAmount);
	else if (key=='f') // move eye +z
		adjustCenter(0.0, 0.0, -adjAmount);*/
}
