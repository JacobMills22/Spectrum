#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilePlayer.h"
#include "Visualisations\Cube.h"
#include "Visualisations\SpectralFade.h"

class AudioVisualiser : public Component,
						public OpenGLRenderer,
						public Timer
{
public:

/** Constructor, specify the number of band to draw*/
	AudioVisualiser(int bands);

/** Destructor */
	~AudioVisualiser();

/** Standard JUCE paint function*/
	void paint(Graphics& g) override;

/** Called when an OpelGL Context is Created 
	used to set standard OpenGL parameters such as
	Viewport and MatrixMode	*/
	void newOpenGLContextCreated() override;
	
/** Main OpenGL rendering function*/
	void renderOpenGL() override;

/** *Called when the OpenGl Context is removed */
	void openGLContextClosing() override;

/** Periodic callback on Message Thread 
	Mainly used to change non-crucial parameters
	such as rotation.	*/
	void timerCallback() override;

/** Sets the SpectrumData, provided by FFT spectral data*/
	void setSpectrumData(int index, float value);

/** Rotates The visualiser on a specified axis*/
	void rotateVisualiser(int axis);

/** *Sets the size/bounds in which OpenGL will Render */
	void setSize(int width, int height);

/** Sets which visualisation should be drawn */
	void setVisualisationToDraw(int index);

	/** Parameter Getters & Setters*/

	void setHorizontalRotationMin(float value);

	void setHorizontalRotationMax(float value);

	void setVerticleRotationMin(float value);

	void setVerticleRotationMax(float value);

	void setRotatingXState(bool state);

	void setRotatingYState(bool state);

	void setRotationSpeed(float value);

	float getRotationSpeed();

	void setNumOfBandsToRender(int bands);

	int getNumOfBandsToRender();

	void setBandDecay(int value);

	
	enum { spectralCubeID = 1, dropletID };

private:

	OpenGLContext openGLContext;

	int width = 600;
	int height = 500;
	

	struct Rotation
	{
		float value;
		float min;
		float max;
		bool invertRotation = false;
	};
	
	enum {xAxis, yAxis, NumofAxis};
	Rotation rotation[NumofAxis];
	bool rotationState[NumofAxis];

	Random RandomNumGen;

	Cube cube;
	Droplet droplet;

	int numOfBands = 8;
	int bandDecay = 10;
	float rotationSpeed = 1.0;
};