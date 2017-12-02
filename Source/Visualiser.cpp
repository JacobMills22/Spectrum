
#pragma once

#include "Visualiser.h"


AudioVisualiser::AudioVisualiser(int bands) : cube(bands), spectralFade(bands)
	{
		// OpenGL Initialisation.
		openGLContext.setRenderer(this);
		openGLContext.attachTo(*this);
		openGLContext.setContinuousRepainting(true);

		// Set Rotation Position.
		rotation[xAxis].value = 0.0;
		rotation[xAxis].min = -20.0;
		rotation[xAxis].max = 50.0;

		rotation[yAxis].value = 0.0;
		rotation[yAxis].min = 80.0;
		rotation[yAxis].max = 120;
		
		// Set Number of bands for the cube to render.
		cube.setNumOfBands(bands);
		spectralFade.setNumOfBands(bands);

		// Start timer at a rate of 50ms.
		startTimer(50);
	}

	AudioVisualiser::~AudioVisualiser()
	{
		// Stop OpenGL from painting and stop timer.
		openGLContext.setContinuousRepainting(false);
		stopTimer();
	}

	void AudioVisualiser::paint(Graphics& g) 
	{
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

//		cube.renderVisualisation(width * 0.5, height * 0.5, -500, 300); // Draw Cube.
		spectralFade.renderVisualisation(width * 0.5, height * 0.5, -500, 300); // Draw Cube.
		
		glPopMatrix();	// Load the transformed matrix data.

	//	zoom = 1.2;
	//	if (zoom >= 1.4) { zoom = 0.0; }
	}

	void AudioVisualiser::openGLContextClosing() 
	{
	}

	void AudioVisualiser::timerCallback()
	{
		rotateVisualiser(xAxis);
		//rotateVisualiser(yAxis);
	}

	void AudioVisualiser::setSpectrumData(int Index, float Value)
	{
		if (Value > 200 || Value <= 0.0)
		{
			cube.setSpectrumData(Index, 0.0);
			spectralFade.setSpectrumData(Index, 0.0);
		}
		else
		{
			if (Value < cube.getSpectrumData(Index))
			{
				if (cube.getSpectrumData(Index) - bandDecay < Value)
				{
					cube.setSpectrumData(Index, Value);
					spectralFade.setSpectrumData(Index, Value);
				}
				else
				{
					cube.setSpectrumData(Index, cube.getSpectrumData(Index) - bandDecay);
					spectralFade.setSpectrumData(Index, Value);
				}
			}
			else
			{
				cube.setSpectrumData(Index, Value);
				spectralFade.setSpectrumData(Index, Value);

			}
		}
	}

	void AudioVisualiser::rotateVisualiser(int axis)
	{
		if (rotation[axis].invertRotation == true)
		{
			rotation[axis].value -= 0.25;
			if (rotation[axis].value <= rotation[axis].min) { rotation[axis].invertRotation = false; }
		}

		if (rotation[axis].invertRotation == false)
		{
			rotation[axis].value += 0.25;
			if (rotation[axis].value >= rotation[axis].max) { rotation[axis].invertRotation = true; }
		}
	}

	void AudioVisualiser::setSize(int Width, int Height)
	{
		width = Width;
		height = Height;
	}



