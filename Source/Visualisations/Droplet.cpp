
#include "Droplet.h"
#include <math.h>

void Droplet::renderVisualisation(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength)
{
	float yModifier = 0.0; // Increments the Y position of each Circle.
	float colourInc = 0.0; // Increments a colour value of each circle to apply a gradient.
	float height = 400.0;  // Height of the visualisation as a whole.
	for (float circles = 0; circles < getNumOfBands(); circles++) // for Each circle/band.
	{
		yModifier += height / (float)getNumOfBands(); // increment the height of the next circle by the height/the total of bands.
		colourInc = circles / (float)getNumOfBands(); // apply colour gradient.
		glBegin(GL_LINE_LOOP);	// Start drawing lines
		// Set colour to the colour of the 1st colour selector + the colour of the 2nd colour selector * the colour increment (gradient).
		glColor3f(getRed(0) + (getRed(1)  * colourInc), getGreen(0) + (getGreen(1)  * colourInc), getBlue(0) + (getBlue(1) * colourInc)); 
		for (int i = 0; i <= 100; i++) 
		{	// Draw circle with 100 verticies.
			double angle = 2 * float_Pi * i / 100;
			double x = cos(angle) * (400 * getSpectrumData(circles)) * 0.0025;
			double y = sin(angle) * (300 * getSpectrumData(circles)) * 0.0025;
			glVertex3f(centerX + x, centerY * 0.25 + yModifier, centerZ + y);
		}
		glEnd();
	}
	
}