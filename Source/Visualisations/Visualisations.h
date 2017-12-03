#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioVisualisation
{
public:

	AudioVisualisation(int bands, int id);

/** OpenGL Drawing Code for a visualisation goes here */
	virtual void renderVisualisation(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength) = 0;
	
/** Sets the SpectrumData for a specifc band, provided by FFT spectral data*/
	void setSpectrumData(int index, float value, int bandDecay);

/** Gets the SpectrumData of a specifc band, provided by FFT spectral data*/
	float getSpectrumData(int Index);

/** Sets the number of bands to draw*/
	void setNumOfBands(int value);

/** Gets the number of bands to draw*/
	int getNumOfBands();

	void setDrawingState(bool state);

	bool getDrawingState();

	void setID(int id);

	int getID();


private:
	
	Random randomNumGen;
	int numOfBands = 0;
	Array<float> spectrumData;
	float ColourIncrement = 0.0;
	String visualisationName;
	bool drawState = false;
	int visualisationID;
};




