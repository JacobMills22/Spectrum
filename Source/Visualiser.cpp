
#pragma once

#include "Visualiser.h"
#include "Visualisations\Cube.h"

AudioVisualiser::AudioVisualiser(int bands) : cube(bands)
	{
		// OpenGL Initialisation.
		openGLContext.setRenderer(this);
		openGLContext.attachTo(*this);
		openGLContext.setContinuousRepainting(true);

		// Set Rotation Position.
		RotationX = 10.0;
		RotationY = 100.0;

		// Set Number of bands for the cube to render.
		cube.setNumOfBands(bands);

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
		glRotatef(RotationX, 1, 0, 0);
		glRotatef(RotationY, 0, 1, 0);
		glTranslatef(-width * 0.5, -height * 0.5, 500);

		cube.drawSpectrumCube(width * 0.5, height * 0.5, -500, 300);
		
		glPopMatrix();
	}

	void AudioVisualiser::openGLContextClosing() 
	{
		DBG("OpenGL Context CLosing");
	}

	void AudioVisualiser::timerCallback()
	{
	//	RotationX += 5;
	//	RotationY += 5;
	}

	void AudioVisualiser::SetZModifier(int Index, float Value)
	{
		if (Value > 1000 || Value <= 0.0)
		{
			cube.setZModifier(Index, 0.0);
		}
		else
		{
			cube.setZModifier(Index, Value);
		}
	}

	void AudioVisualiser::setSize(int Width, int Height)
	{
		width = Width;
		height = Height;
	}



