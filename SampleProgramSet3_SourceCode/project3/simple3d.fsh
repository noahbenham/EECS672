#version 410 core

// Custom int values for perspective type, matches ModelViewWithPhongLighting.h
#define C_PERSPECTIVE 0
#define C_ORTHOGONAL 1
#define C_OBLIQUE 2

in vec3 p_ecPosition;
in vec3 ec_nHat;

out vec4 fragmentColor;

const int MAX_NUM_LIGHTS = 3;

uniform int numLights;
uniform int projType;
uniform vec4 lightPosition[MAX_NUM_LIGHTS];
uniform vec3 lightStrength[MAX_NUM_LIGHTS];
uniform vec3 globalAmbient;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float m;


vec3 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat) {
	vec3 diffuseTot = vec3(0.0, 0.0, 0.0);
	vec3 specularTot = vec3(0.0, 0.0, 0.0);

	vec3 vHat;
	if (projType == C_PERSPECTIVE)
		vHat = -(normalize(ec_Q));
	else if (projType == C_ORTHOGONAL)
		vHat = vec3(0.0, 0.0, 1.0); // already normalized
	else if (projType == C_OBLIQUE)
		vHat = vec3(0.0, 0.0, 1.0); // already normalized

	if (dot(ec_nHat, vHat) < 0)
		ec_nHat = -ec_nHat;

	for (int i=0; i < numLights; i++) {
		vec3 LiHat = vec3(0.0, 0.0, 0.0);
		vec4 currLightPos = lightPosition[i];

		if (currLightPos.w == 0.0)
			LiHat = normalize(currLightPos.xyz);
		else
			LiHat = normalize(currLightPos.xyz - ec_Q);

		if (dot(LiHat, ec_nHat) > 0) { // value cannot be negative
			vec3 riHat = normalize(reflect(vHat, ec_nHat));
			float riDotV = dot(riHat, vHat);

			float atten = 1; // by default, do not take into account
			if (lightPosition[i][3] == 1) { // is point light src
				float d = distance(lightPosition[i].xyz, ec_Q);
				atten = 1 / d;
			}

			if (riDotV > 0)
				specularTot += atten * lightStrength[i] * (ks * pow(riDotV, m));
			diffuseTot += atten * lightStrength[i] * (kd * dot(ec_nHat, LiHat));
		}
	}

	vec3 returnVal = ka * globalAmbient + (diffuseTot + specularTot);

	// Prevent any component of returnVal from going above 1
	float maxVal = 0.0;
	for (int i=0; i < returnVal.length(); i++)
		if (returnVal[i] > maxVal)
			maxVal = returnVal[i]; // set new max value

	if (maxVal > 1.0)
		returnVal = returnVal / maxVal;

	return returnVal;
}

void main() {
	vec3 colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);
	fragmentColor = vec4(colorToFS, 1.0);
}
