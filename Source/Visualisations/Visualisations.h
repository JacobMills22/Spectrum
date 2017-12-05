#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class AudioVisualisation
{
public:

/** Class contains all nesseaary functions to render an OpenGL Visualisation.
	SpectrumData will need to be set periodically by FFT data.	*/
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

/** Sets whether or not a visualisation should be drawn*/
	void setDrawingState(bool state);

/** Gets whether or not a visualisation should be drawn*/
	bool getDrawingState();

/** Sets the colour of a specific element of a visualisation (specified by index) */
	void setRenderColour(int index, float red, float green, float blue);

/** Gets the current RGB colour data*/
	float getRed(int index);
	float getGreen(int index);
	float getBlue(int index);
	
private:
	
	int numOfBands = 0;
	Array<float> spectrumData;
	bool drawState = false;
	int visualisationID;

	struct ColoursRGB
	{
		float red = 0.0;
		float green = 0.0;
		float blue = 0.0;
	};

	ColoursRGB VisualisationColour[2];
};




