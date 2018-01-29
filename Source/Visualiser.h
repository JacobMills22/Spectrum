#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FilePlayer.h"


//Windows
//#include "Visualisations\Cube.h"
//#include "Visualisations\Droplet.h"

// Mac
#include "Visualisations/Cube.h"
#include "Visualisations/Droplet.h"


/** Handles the OpenGL rendering of the project.
Inherits from an OpenGLRender which supplies useful rendering
functions. Has an OpenGL context which allows the OpenGL rendering
to be attach to this classes component.
Cube and Droplet handle the drawing code.
*/
class AudioVisualiser : public Component,
						public OpenGLRenderer,
						public Timer
{
public:

/** Constructor, specify the number of band to draw.
	Initialises OpenGL context and other variables.
	@param bands: the number of bands to draw */
	AudioVisualiser(int bands);

/** Destructor, stops the timer and stops OpenGL
	from rendering. */
	~AudioVisualiser();

/** Called when an OpenGL Context is Created 
	used to set standard OpenGL parameters such as
	Viewport and MatrixMode	*/
	void newOpenGLContextCreated() override;
	
/** Main OpenGL rendering function*/
	void renderOpenGL() override;

/** Called when the OpenGl Context is removed */
	void openGLContextClosing() override;

/** Periodic callback on message thread 
	Mainly used to change non-crucial parameters
	such as rotation.	*/
	void timerCallback() override;

/** Rotates The visualiser on a specified axis.
	@param axis: the axis to rotate on, (use enum {xAxis, yAxis, numOfAxis}) */
	void rotateVisualiser(int axis);

	/** Getters & Setters*/

/** Sets the SpectrumData, provided by FFT spectral data 
	@param index: the band number to be set.
	@param value: the data to set. */
	void setSpectrumData(int index, float value);

/** *Sets the size/bounds in which OpenGL will Render */
	void setSize(int width, int height);

/** Sets which visualisation should be drawn 
	@param index: Use enum {spectralCubeID = 1, dropletID} */
	void setVisualisationToDraw(int index);
		
/** Sets the horizontal minimum value that the visualiser will rotate to*/
	void setHorizontalRotationMin(float value);

/** Sets the horizontal maximum value that the visualiser will rotate to*/
	void setHorizontalRotationMax(float value);

/** Sets the vertical minimum value that the visualiser will rotate to*/
	void setVerticleRotationMin(float value);

/** Sets the vertical maximum value that the visualiser will rotate to*/
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

/** Sets the render colour of a specified element of the visualiser.
	@param index: Which visualisation element will change colour (0/1)  */
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
