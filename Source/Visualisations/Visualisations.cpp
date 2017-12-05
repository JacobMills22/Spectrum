
#include "Visualisations.h"

AudioVisualisation::AudioVisualisation(int bands, int id)
{
	setNumOfBands(bands);	// Set the number of audio bands for the cube to render.
	spectrumData.resize(bands); // resizes the Z Modifier Array to the number of bands.

	for (int band = 0; band < bands; band++)
	{
		setSpectrumData(band, 0.0, 0);	// Initialises all bands to 0.
	}
}

void AudioVisualisation::setSpectrumData(int index, float value, int bandDecay)
{
	if (value > 200 || value <= 0.0)	// Removes noise values.
	{
		spectrumData.set(index, 0.0);
	}
	else
	{
		if (value < getSpectrumData(index)) // If new FFT Value is less than the old
		{
			if (value > getSpectrumData(index) - bandDecay ) // if new FFT value is greater than the old - the decay amount
			{
				spectrumData.set(index, value); // Set the spectrum data to the new value
			}
			else // Otherwise set the spectrum value to the old value minuse - the decay
			{	
				spectrumData.set(index, getSpectrumData(index) - bandDecay); 
			}
		}
		else // Set spectrum data to the new value if the new FFT Value is greater than the old
		{
			spectrumData.set(index, value);
		}
	}

}

float AudioVisualisation::getSpectrumData(int Index)
{
	return spectrumData[Index];
}

int AudioVisualisation::getNumOfBands()
{
	return numOfBands;
}

void AudioVisualisation::setNumOfBands(int value)
{
	numOfBands = value;
}

void AudioVisualisation::setDrawingState(bool state)
{
	drawState = state;
}

bool AudioVisualisation::getDrawingState()
{
	return drawState;
}


void AudioVisualisation::setRenderColour(int index, float red, float green, float blue)
{
	VisualisationColour[index].red = red;
	VisualisationColour[index].green = green;
	VisualisationColour[index].blue = blue;
}

float AudioVisualisation::getRed(int index)
{
	return VisualisationColour[index].red;
}

float AudioVisualisation::getGreen(int index)
{
	return VisualisationColour[index].green;
}

float AudioVisualisation::getBlue(int index)
{
	return VisualisationColour[index].blue;
}
