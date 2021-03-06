#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Visualisations.h"

#include <OpenGL/gl.h>


/** OpenGL Drawing code for the Droplet visualisation.
Inherits from AudioVisualisation. 
@see AudioVisualisation
*/
class Droplet : public AudioVisualisation
{
	using AudioVisualisation::AudioVisualisation; // Construct with the AudioVisualisations constructor

public:

/** Renders an OpenGL Cube with spectral visualisations set by spectrumData.*/
	void renderVisualisation(GLfloat centerX, GLfloat centerY, GLfloat centerZ, GLfloat edgeLength) override;

private:

};
