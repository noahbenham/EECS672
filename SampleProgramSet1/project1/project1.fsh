// Noah Benham
// Project 1
#version 420 core

uniform int vertexColor;
out vec4 fragmentColor;

void main()
{
	vec4 colorChoices[6] = {
		{ 1.0, 1.0, 1.0, 1.0 }, // white
		{ 0.0, 0.0, 0.0, 1.0 }, // black
		{ 0.0, 0.0, 1.0, 1.0 }, // blue
		{ 1.0, 0.0, 0.0, 1.0 }, // red
		{ 0.0, 1.0, 0.0, 1.0 }, // green
		{ 0.7, 0.0, 1.0, 1.0 }}; // purple

	if (vertexColor < 6)
		fragmentColor = colorChoices[vertexColor];
	else
		fragmentColor = colorChoices[1];
}

