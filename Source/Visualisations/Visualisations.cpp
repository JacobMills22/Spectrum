
#include "Visualisations.h"

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