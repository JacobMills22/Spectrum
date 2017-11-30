#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class Cube
{

public:

	Cube(int bands);

	void drawSpectrumCube(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength);

	void setZModifier(int Index, float Value);

	float getZModifier(int Index);

	int getNumOfBands();

	void setNumOfBands(int value);

private:

	Random randomNumGen;
	int numOfBands = 0;
	Array<float> frontZModifier;

};