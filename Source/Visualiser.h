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
	void setSpectrumData(int Index, float Value);

	void rotateVisualiser(int axis);

/** *Sets the size/bounds in which OpenGL will Render */
	void setSize(int width, int height);


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

	Random RandomNumGen;

	Cube cube;
	SpectralFade spectralFade;

	int numOfBands = 0;
	int bandDecay = 10;

	float zoom = 0;		// temp temp temp
};