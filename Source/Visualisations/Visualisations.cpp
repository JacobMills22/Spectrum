
#include "Visualisations.h"

AudioVisualisation::AudioVisualisation(int bands, int id)
{
	setNumOfBands(bands);	// Set the number of audio bands for the cube to render.
	spectrumData.resize(bands); // resizes the Z Modifier Array to the number of bands.
	setID(id);

	for (int band = 0; band < bands; band++)
	{
		setSpectrumData(band, 0.0, 0);	// Initialises all bands to 0.
	}

	ColourIncrement = 0.9 / bands;
}

void AudioVisualisation::setSpectrumData(int index, float value, int bandDecay)
{
//	spectrumData.set(index, value);


	if (value > 200 || value <= 0.0)
	{
		spectrumData.set(index, 0.0);
	}
	else
	{
		if (value < getSpectrumData(index))
		{
			if (getSpectrumData(index) - bandDecay < value)
			{
				spectrumData.set(index, value);
			}
			else
			{
				spectrumData.set(index, getSpectrumData(index) - bandDecay);
			}
		}
		else
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

void AudioVisualisation::setID(int id)
{
	visualisationID = id;
}

int AudioVisualisation::getID()
{
	return visualisationID;
}