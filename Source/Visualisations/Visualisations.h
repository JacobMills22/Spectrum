#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioVisualisation
{
public:

	AudioVisualisation(int bands);

/** OpenGL Drawing Code for a visualisation goes here */
	virtual void renderVisualisation(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength) = 0;
	
/** Sets the SpectrumData for a specifc band, provided by FFT spectral data*/
	void setSpectrumData(int Index, float Value);

/** Gets the SpectrumData of a specifc band, provided by FFT spectral data*/
	float getSpectrumData(int Index);

/** Sets the number of bands to draw*/
	void setNumOfBands(int value);

/** Gets the number of bands to draw*/
	int getNumOfBands();

private:
	
	Random randomNumGen;
	int numOfBands = 0;
	Array<float> spectrumData;
	float ColourIncrement = 0.0;
};




