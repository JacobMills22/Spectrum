
#include "Visualisations.h"

AudioVisualisation::AudioVisualisation(int bands)
{
	setNumOfBands(bands);	// Set the number of audio bands for the cube to render.
	spectrumData.resize(bands); // resizes the Z Modifier Array to the number of bands.

	for (int band = 0; band < bands; band++)
	{
		setSpectrumData(band, 0.0);	// Initialises all bands to 0.
	}

	ColourIncrement = 0.9 / bands;
}

void AudioVisualisation::setSpectrumData(int Index, float Value)
{
	spectrumData.set(Index, Value);
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