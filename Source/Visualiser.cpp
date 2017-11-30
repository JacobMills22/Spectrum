
#pragma once

#include "Visualiser.h"
#include "Visualisations\Cube.h"

AudioVisualiser::AudioVisualiser(int bands) : cube(bands)
	{
		openGLContext.setRenderer(this);
		openGLContext.attachTo(*this);
		openGLContext.setContinuousRepainting(true);

		RotationX = 10.0;
		RotationY = 100.0;

		cube.setNumOfBands(bands);

		startTimer(50);
	}

	AudioVisualiser::~AudioVisualiser()
	{
		openGLContext.setContinuousRepainting(false);
		stopTimer();
	}

	void AudioVisualiser::paint(Graphics& g) 
	{
	}

	void AudioVisualiser::newOpenGLContextCreated() 
	{
		glViewport(0.0f, 0.0f, width, height); // specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
		glMatrixMode(GL_PROJECTION); // projection matrix defines the properties of the camera that views the objects in the world coordinate frame. Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
		glLoadIdentity(); // replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
		glOrtho(0.0, width, 0, height, 0.0, 1000.0); // essentially set coordinate system
		glMatrixMode(GL_MODELVIEW); // (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
		glLoadIdentity(); // same as above comment
	}



	void AudioVisualiser::renderOpenGL() 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glDepthRange(0.0, 2000.0);

		glPushMatrix();
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



