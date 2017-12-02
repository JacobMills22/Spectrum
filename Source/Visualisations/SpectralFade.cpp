
#include "SpectralFade.h"
#include <math.h>

SpectralFade::SpectralFade(int bands)
{
	setNumOfBands(bands);	// Set the number of audio bands for the cube to render.
	spectrumData.resize(bands); // resizes the Z Modifier Array to the number of bands.

	for (int band = 0; band < bands; band++)
	{
		setSpectrumData(band, 0.0);	// Initialises all bands to 0.
	}

	ColourIncrement = 0.9 / bands;
}

void SpectralFade::renderVisualisation(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength)
{
	float yModifier = 0.0;
	float green = 0.0;
	for (float circles = 0; circles < getNumOfBands(); circles++)
	{
		yModifier += getNumOfBands() * 0.12;
		green = circles / (float)getNumOfBands();
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0, green, 1.0);
		for (int i = 0; i <= 100; i++)
		{
			double angle = 2 * float_Pi * i / 100;
			double x = cos(angle) * (400 * getSpectrumData(circles)) * 0.0025;
			double y = sin(angle) * (300 * getSpectrumData(circles)) * 0.0025;
			glVertex3f(centerX + x, centerY * 0.25 + yModifier, centerZ + y);
		}
		glEnd();
	}
	
}