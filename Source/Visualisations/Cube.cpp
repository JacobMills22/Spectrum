
#include "Cube.h"


Cube::Cube(int bands)
{
	setNumOfBands(bands);
	frontZModifier.resize(bands);

	for (int band = 0; band < bands; band++)
	{
		setZModifier(band, 0.0);
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
	glBegin(GL_QUADS);
		glColor3f(0.4f, 0.0f, 0.0f);
		glVertex3f(cubeLeft,  cubeTop, cubeFront);	  // front left
		glVertex3f(cubeLeft,  cubeTop, cubeBack);	  // back left
		glVertex3f(cubeRight, cubeTop, cubeBack);	  // back Right
		glVertex3f(cubeRight, cubeTop, cubeFront);	  // front Right

	// Bottom
		glColor3f(0.2f, 0.0f, 0.0f);
		glVertex3f(cubeLeft,  cubeBottom, cubeFront); // front left
		glVertex3f(cubeLeft,  cubeBottom, cubeBack);  // back left
		glVertex3f(cubeRight, cubeBottom, cubeBack);  // back Right
		glVertex3f(cubeRight, cubeBottom, cubeFront); // front Right

	// Left
		glColor3f(0.1f, 0.0f, 0.0f);
		glVertex3f(cubeLeft, cubeTop,    cubeFront);  // front left
		glVertex3f(cubeLeft, cubeTop,    cubeBack);	  // back left
		glVertex3f(cubeLeft, cubeBottom, cubeBack);	  // back Right
		glVertex3f(cubeLeft, cubeBottom, cubeFront);  // front Right
	
	// Right
		glColor3f(0.3f, 0.0f, 0.0f);
		glVertex3f(cubeRight, cubeTop,    cubeFront);	//front left
		glVertex3f(cubeRight, cubeTop,    cubeBack);	// back left
		glVertex3f(cubeRight, cubeBottom, cubeBack);	// back Right
		glVertex3f(cubeRight, cubeBottom, cubeFront);	// front Right
	glEnd();

	float quadHeight = edgeLength * (0.5 / getNumOfBands());
	float bandIncrement = 0.0;

	// Front
	for (int band = 0; band < getNumOfBands(); band++)
	{
		glBegin(GL_QUADS);
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement, cubeFront);
			glVertex3f(cubeRight, cubeBottom + bandIncrement, cubeFront);
			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight, cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band));
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band));
		
			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight, cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band));
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement + quadHeight, cubeFront + getZModifier(band));
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft,  cubeBottom + bandIncrement + (quadHeight * 2), cubeFront);
			glVertex3f(cubeRight, cubeBottom + bandIncrement + (quadHeight * 2), cubeFront);
		glEnd();

		bandIncrement += quadHeight * 2;
	}
	bandIncrement = 0.0;

	// back
	for (int Point = 0; Point < getNumOfBands(); Point++)
	{
		glBegin(GL_LINE_LOOP);
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft + 1,  cubeBottom + bandIncrement, cubeBack);	// back left
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement, cubeBack);	// back Right
			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));
			glVertex3f(cubeLeft + 1,  cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));

			glColor3f(0.0f, randomNumGen.nextFloat(), 1.0f);
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));	// back left
			glVertex3f(cubeLeft  + 1, cubeBottom + bandIncrement + quadHeight, cubeBack - getZModifier(Point));	// back Right
			glColor3f(0.2f, 0.0f, 0.0f);
			glVertex3f(cubeLeft  + 1, cubeBottom + bandIncrement + (quadHeight * 2), cubeBack);	// front Right
			glVertex3f(cubeRight - 1, cubeBottom + bandIncrement + (quadHeight * 2), cubeBack);	// front Left
		glEnd();

		bandIncrement += quadHeight * 2;
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
