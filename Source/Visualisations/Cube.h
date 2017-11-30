#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class Cube
{

public:
/** Constructs the cube with a specified number of bands.*/
	Cube(int bands);

/** Renders an OpenGL Cube with spectral visualisations set by the Z Modifier.*/
	void drawSpectrumCube(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength);

/** Sets the Z Modifer for the one of the cubes audio visualisation bands*/
	void setZModifier(int Index, float Value);

/** Gets the Z Modifer for the one of the cubes audio visualisation bands*/
	float getZModifier(int Index);

/** Sets the number of bands the Cube is Rendering.*/
	void setNumOfBands(int value);

/** Gets the number of bands the Cube is Rendering.*/
	int getNumOfBands();


private:

	Random randomNumGen;
	int numOfBands = 0;
	Array<float> frontZModifier;
	float ColourIncrement = 0.0;

};