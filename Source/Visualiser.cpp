
//#pragma once

#include "Visualiser.h"


AudioVisualiser::AudioVisualiser(int bands) : cube(bands, spectralCubeID), droplet(bands, dropletID)
	{
		// OpenGL Initialisation.
		openGLContext.setRenderer(this);
		openGLContext.attachTo(*this);
		openGLContext.setContinuousRepainting(true);

		// Set Rotation Position.
		rotation[xAxis].value = 0.0;
		rotation[xAxis].min = 0.0;
		rotation[xAxis].max = 0.0;

		rotation[yAxis].value = 0.0;
		rotation[yAxis].min = 0.0;
		rotation[yAxis].max = 0.0;
		
		// Set Number of bands for the cube to render.
		cube.setNumOfBands(bands);
		droplet.setNumOfBands(bands);

		// Start timer at a rate of 50ms.
		startTimer(50);
	}

	AudioVisualiser::~AudioVisualiser()
	{
		// Stop OpenGL from painting and stop timer.
		openGLContext.setContinuousRepainting(false);
		stopTimer();
	}

	void AudioVisualiser::newOpenGLContextCreated() 
	{
		glViewport(0.0f, 0.0f, width, height); // Set the Position, Width and Height of the Window for OpenGL to render in.
		glMatrixMode(GL_PROJECTION); // Set matrix mode to Projection, so that the camera properties can be modified.
		glLoadIdentity(); // Reset Matrix to Default state (Just in case any rotation or transalation takes place).
		glOrtho(0.0, width, 0, height, 0.0, 1000.0); // Set the space for the coordinate system.
		glMatrixMode(GL_MODELVIEW); // Set Matrix mode back to standar Modelview so that transform properties such as rotation can be modified.
		glLoadIdentity(); // Reset Matrix to Default state (Just in case any rotation or transalation takes place).
		
	}
	
	void AudioVisualiser::renderOpenGL() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Colour and Depth Buffers.

		glEnable(GL_DEPTH_TEST);	// Enable and set depth range, This allows the OpenGL 
		glDepthFunc(GL_LESS);		// Context to use its depth buffer. Without this OpenGL will
		glDepthRange(0.0, 2000.0);	// not be able to render 3D shapes correctly.

		// Center rendering to the center of the window and rotate if needed.
		glPushMatrix();	// Sets the current Matrix to apply transformations.
		glTranslatef(width * 0.5, height * 0.5, -500);
		glRotatef(rotation[xAxis].value, 1, 0, 0);
		glRotatef(rotation[yAxis].value, 0, 1, 0);
		glRotatef(0, 0, 0, 1);
		glTranslatef(-width * 0.5, -height * 0.5, 500);

		if (cube.getDrawingState() == true) { cube.renderVisualisation(width * 0.5, height * 0.5, -500, 300); } // Draw Cube.
		else if (droplet.getDrawingState() == true) {droplet.renderVisualisation(width * 0.5, height * 0.5, -500, 300); } // Draw Droplet.
		
		glPopMatrix();	// Load the transformed matrix data.
	}

	void AudioVisualiser::openGLContextClosing() 
	{
	}

	void AudioVisualiser::timerCallback()
	{
		if (rotationState[xAxis] == true) { rotateVisualiser(xAxis); }	// Rotate if required.
		if (rotationState[yAxis] == true) { rotateVisualiser(yAxis); }  // Rotate if required.
	}

	void AudioVisualiser::setSpectrumData(int index, float value)
	{
		if (cube.getDrawingState() == true) { cube.setSpectrumData(index, value, bandDecay); }       // Set current spectrum data if cube is being drawn
		if (droplet.getDrawingState() == true) { droplet.setSpectrumData(index, value, bandDecay); } // Set current spectrum data if droplet is being drawn
	}

	void AudioVisualiser::rotateVisualiser(int axis)
	{   // Rotate visualiser with a "Ping-Pong" style.
		if (rotation[axis].invertRotation == true)		// if visualiser should be rotating backwards
		{
			rotation[axis].value -= getRotationSpeed(); // Decrement the rotation value by the current speed of rotation.
			if (rotation[axis].value <= rotation[axis].min) { rotation[axis].invertRotation = false; } // Once the value reaches the minimum
		}																							   // set the visualiser to rotate forwards.

		if (rotation[axis].invertRotation == false)		// if visualiser should be rotating forwards
		{
			rotation[axis].value += getRotationSpeed(); // Increment the rotation value by the current speed of rotation.
			if (rotation[axis].value >= rotation[axis].max) { rotation[axis].invertRotation = true; } // Once the value reaches the maximum
		}																						      // set the visualiser to rotate backwards.
	}

	void AudioVisualiser::setSize(int Width, int Height)
	{
		width = Width;
		height = Height;
	}

	void AudioVisualiser::setVisualisationToDraw(int index)
	{
		cube.setDrawingState(false);
		droplet.setDrawingState(false);

		if (index == spectralCubeID)
		{
			cube.setDrawingState(true);
		}
		else if (index == dropletID)
		{
			droplet.setDrawingState(true);
		}
	}

	void AudioVisualiser::setHorizontalRotationMin(float value)
	{
		rotation[xAxis].min = value;
		rotation[xAxis].value = value;
	}

	void AudioVisualiser::setHorizontalRotationMax(float value)
	{
		rotation[xAxis].max = value;
		rotation[xAxis].value = value;
	}

	void AudioVisualiser::setVerticleRotationMin(float value)
	{
		rotation[yAxis].min = value;
		rotation[yAxis].value = value;
	}

	void AudioVisualiser::setVerticleRotationMax(float value)
	{
		rotation[yAxis].max = value;
		rotation[yAxis].value = value;
	}

	void AudioVisualiser::setRotatingXState(bool state)
	{
		rotationState[xAxis] = state;
	}

	void AudioVisualiser::setRotatingYState(bool state)
	{
		rotationState[yAxis] = state;
	}

	void AudioVisualiser::setRotationSpeed(float value)
	{
		rotationSpeed = value;
	}

	float AudioVisualiser::getRotationSpeed()
	{
		return rotationSpeed;
	}

	void AudioVisualiser::setNumOfBandsToRender(int bands)
	{
		numOfBands = bands;
		cube.setNumOfBands(bands);
		droplet.setNumOfBands(bands);
	}

	int AudioVisualiser::getNumOfBandsToRender()
	{
		return numOfBands;
	}

	void AudioVisualiser::setBandDecay(int value)
	{
		bandDecay = value;
	}

	void AudioVisualiser::setRenderColour(int index, float red, float green, float blue)
	{
		cube.setRenderColour(index, red, green, blue);
		droplet.setRenderColour(index, red, green, blue);
	}



