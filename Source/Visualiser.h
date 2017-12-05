#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilePlayer.h"
#include "Visualisations\Cube.h"
#include "Visualisations\Droplet.h"

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

/** Called when an OpenGL Context is Created 
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

/** Rotates The visualiser on a specified axis*/
	void rotateVisualiser(int axis);

	/** Getters & Setters*/

/** Sets the SpectrumData, provided by FFT spectral data*/
	void setSpectrumData(int index, float value);

/** *Sets the size/bounds in which OpenGL will Render */
	void setSize(int width, int height);

/** Sets which visualisation should be drawn */
	void setVisualisationToDraw(int index);
		
/** Sets the horizontal minimum value that the visualiser will rotate to*/
	void setHorizontalRotationMin(float value);

/** Sets the horizontal maximum value that the visualiser will rotate to*/
	void setHorizontalRotationMax(float value);

/** Sets the verticle minimum value that the visualiser will rotate to*/
	void setVerticleRotationMin(float value);

/** Sets the verticle maximum value that the visualiser will rotate to*/
	void setVerticleRotationMax(float value);

/** Sets whether or not the visualiser should be rotating on the X axis*/
	void setRotatingXState(bool state);

/** Sets whether or not the visualiser should be rotating on the Y axis*/
	void setRotatingYState(bool state);

/** Sets the speed at which the visualiser rotates*/
	void setRotationSpeed(float value);

/** Gets the current rotation speed of the visualiser*/
	float getRotationSpeed();

/** Sets the number of bands that the visuliser will render*/
	void setNumOfBandsToRender(int bands);

/** Gets the number of bands that the visuliser will render*/
	int getNumOfBandsToRender();

/** Sets how quickly each band will fall back down to 0*/
	void setBandDecay(int value);

/** Sets the render colour of a specified element of the visualiser*/
	void setRenderColour(int index, float red, float green, float blue);
		
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
	
	enum {xAxis, yAxis, numOfAxis};
	Rotation rotation[numOfAxis];
	bool rotationState[numOfAxis];

	Cube cube;
	Droplet droplet;

	int numOfBands = 8;
	int bandDecay = 10;
	float rotationSpeed = 1.0;
};