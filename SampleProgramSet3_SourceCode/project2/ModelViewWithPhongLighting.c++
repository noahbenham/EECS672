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
GLint ModelViewWithPhongLighting::ppuLoc_kd = -2;
GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;

GLint ModelViewWithPhongLighting::ppuLoc_lightStrength = -2;
GLint ModelViewWithPhongLighting::ppuLoc_globalAmbient = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ka = -2;

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

float lightStrength[ ] = {0.9, 0.9, 0.9};
float ambientStrength[ ] = {0.35, 0.35, 0.35}; // assumed ambient light
float ka[ ] = {0.3, 0.6, 0.5}; // TODO externalize to individual models later

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
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");

		ppuLoc_lightStrength = ppUniformLocation(shaderProgram, "lightStrength");
		ppuLoc_globalAmbient = ppUniformLocation(shaderProgram, "globalAmbient");
		ppuLoc_ka = ppUniformLocation(shaderProgram, "ka");
	}
}

void ModelViewWithPhongLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc) {
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}

void ModelViewWithPhongLighting::establishLights() {
	int numLights = 1;
	glUniform3fv(ppuLoc_lightStrength, numLights, lightStrength);
	glUniform3fv(ppuLoc_globalAmbient, 1, ambientStrength);
	glUniform3fv(ppuLoc_ka, 1, ka);
}

