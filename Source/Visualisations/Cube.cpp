
#include "Cube.h"


Cube::Cube(int bands)
{
	setNumOfBands(bands);	// Set the number of audio bands for the cube to render.
	frontZModifier.resize(bands); // resizes the Z Modifier Array to the number of bands.

	for (int band = 0; band < bands; band++)
	{
		setZModifier(band, 0.0);	// Initialises all bands to 0.
	}
	
}

void Cube::drawSpectrumCube(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength)
{
	GLfloat halfSideLength = edgeLength * 0.5f;

	GLfloat cubeLeft = centerX - halfSideLength;
	GLfloat cubeRight = centerX + halfSideLength;
	GLfloat cubeBottom = centerY - halfSideLength;
	GLfloat cubeTop = centerY + halfSideLength;
	GLfloat cubeFront = centerZ + halfSideLength;
	GLfloat cubeBack = centerZ - halfSideLength;

	// Top
	glBegin(GL_QUADS);								  // Begin drawing quads.
		glColor3f(0.4f, 0.0f, 0.0f);				  // Set colour (RGB)
		glVertex3f(cubeLeft,  cubeTop, cubeFront);	  // front left
		glVertex3f(cubeLeft,  cubeTop, cubeBack);	  // back left
		glVertex3f(cubeRight, cubeTop, cubeBack);	  // back Right
		glVertex3f(cubeRight, cubeTop, cubeFront);	  // front Right

	// Bottom
		glColor3f(0.2f, 0.0f, 0.0f);				  // Set colour (RGB)
		glVertex3f(cubeLeft,  cubeBottom, cubeFront); // front left
		glVertex3f(cubeLeft,  cubeBottom, cubeBack);  // back left
		glVertex3f(cubeRight, cubeBottom, cubeBack);  // back Right
		glVertex3f(cubeRight, cubeBottom, cubeFront); // front Right

	// Left
		glColor3f(0.1f, 0.0f, 0.0f);                  // Set colour (RGB)
		glVertex3f(cubeLeft, cubeTop,    cubeFront);  // front left
		glVertex3f(cubeLeft, cubeTop,    cubeBack);	  // back left
		glVertex3f(cubeLeft, cubeBottom, cubeBack);	  // back Right
		glVertex3f(cubeLeft, cubeBottom, cubeFront);  // front Right
	
	// Right
		glColor3f(0.3f, 0.0f, 0.0f);                  // Set colour (RGB)
		glVertex3f(cubeRight, cubeTop,    cubeFront); // front left
		glVertex3f(cubeRight, cubeTop,    cubeBack);  // back left
		glVertex3f(cubeRight, cubeBottom, cubeBack);  // back Right
		glVertex3f(cubeRight, cubeBottom, cubeFront); // front Right
	glEnd();

	// Remaining two side of the cube are the spectral visualisations which are made up of seperate
	// audio bands, each band is drawn seperately in a for loop.

	float quadHeight = edgeLength * (0.5 / getNumOfBands()); // Sets the height of all bands scaled by the number of bands.
	float bandIncrement = 0.0;	// Each time a band is drawn an increment is applied, so the next band is drawn at a higher position. 

	// Front
	for (int band = 0; band < getNumOfBands(); band++)	// For each band to render
	{
		glBegin(GL_QUADS);												  // Begin Drawing filled Quads
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement, cubeFront); // Bottom Verticies of 1st quad
			glVertex3f(cubeRight, cubeBottom + bandIncrement, cubeFront); // (not Modulated by audio, act as anchors)
			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight, cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band)); // Top Verticies of 1st quad
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band));	// (Modulated by audio)
		
			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight, cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band)); // Bottom Verticies of 2nd quad
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band));  // (Modulated by audio)
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement + (quadHeight * 2), cubeFront); // Top Verticies of 2nd quad, moved above the 1st Quad
			glVertex3f(cubeRight, cubeBottom + bandIncrement + (quadHeight * 2), cubeFront); // (not Modulated by audio, act as anchors)
		glEnd();														  // Stop Drawing Quads

		bandIncrement += quadHeight * 2; // Two Quads were drawn so the next set need to be drawn higher.
	}
	bandIncrement = 0.0;	// Reset Increment so that the quads on the other side of the cube can be drawn.

	// Back
	for (int Point = 0; Point < getNumOfBands(); Point++) // For each band to render
	{
		glBegin(GL_LINE_LOOP);												 // Begin Drawing Wireframe Quads (Lines)
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft + 1,  cubeBottom + bandIncrement, cubeBack); // Bottom Verticies of 1st quad
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement, cubeBack); // (not Modulated by audio, act as anchors)
			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point)); // Top Verticies of 1st
			glVertex3f(cubeLeft  + 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));	// (Modulated by audio)

			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));	// Top Verticies of 1st
			glVertex3f(cubeLeft  + 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));	// (Modulated by audio)
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft  + 1, cubeBottom + bandIncrement + (quadHeight * 2), cubeBack);	// Top Verticies of 2nd quad, moved above the 1st Quad
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement + (quadHeight * 2), cubeBack);	// (not Modulated by audio, act as anchors)
		glEnd();															 // Stop Drawing Wireframe Quads (Lines)

		bandIncrement += quadHeight * 2; // Two Quads were drawn so the next set need to be drawn higher.
	}
}

void Cube::setZModifier(int Index, float Value)
{
	frontZModifier.set(Index, Value);
}

float Cube::getZModifier(int Index)
{
	return frontZModifier[Index];
}

int Cube::getNumOfBands()
{
	return numOfBands;
}

void Cube::setNumOfBands(int value)
{
	numOfBands = value;
}
