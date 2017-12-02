#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Visualisations.h"

class SpectralFade : public AudioVisualisation
{

public:

	/** Constructs the cube with a specified number of bands.*/
	SpectralFade(int bands);

	/** Renders an OpenGL Cube with spectral visualisations set by the Z Modifier.*/
	void renderVisualisation(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength) override;

private:

	Random randomNumGen;
	int numOfBands = 0;
	Array<float> spectrumData;
	float ColourIncrement = 0.0;

	float ModX = 50;
	float ModY = 50;
};