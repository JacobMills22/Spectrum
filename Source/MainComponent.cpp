/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Visualiser.h"
#include "FilePlayer.h"

#define VisualiserBands 64

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
		                       public Timer
{
public:
    //==============================================================================
    MainContentComponent() : fastFourierTransform(FFTOrder, false),
							 visualiser(VisualiserBands)
    {
        setSize (800, 600);
		setAudioChannels(0, 2);

		addAndMakeVisible(visualiser);
		visualiser.setSize(800, 600);
		addAndMakeVisible(filePlayer);
		fifoIndex = 0;
		nextFFTBlockReady = false;

		convertBandsToLogScale();

		startTimer(30);
    }

    ~MainContentComponent()
    {
		shutdownAudio();
		stopTimer();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
		filePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate); // Tell the filePlayer the samplerate and how many samples to expect.
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
	{
		bufferToFill.clearActiveBufferRegion();		// Clear noise from buffer.

		if (filePlayer.getPlaybackState() == true)	// If the Fileplayer is playing audio.
		{
			filePlayer.getNextAudioBlock(bufferToFill);	// Fill buffer with the next audio block from filePlayer.
		}

		float *outputL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample); // Writeable Left Channel Output Array.
		float *outputR = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample); // Writeable Right Channel Output Array.

		for (int sample = 0; sample < bufferToFill.numSamples; sample++) // For each sample.
		{
			if (filePlayer.getPlaybackState() == true) // If the Fileplayer is playing audio.
			{
				pushNextSampleIntoFifo(outputL[sample]); // Add current sample to fifo array.
			}

			outputL[sample] *= 0.01;	// Scale audio down.
			outputR[sample] *= 0.01;	// Scale audio down.
		}
	}

    void releaseResources() override
    {
		filePlayer.releaseResources();
    }

	void pushNextSampleIntoFifo(float sample) noexcept
	{
		// Once Fifo has enough data set to block state to ready.
		if (fifoIndex >= FFTSize)
		{
			if (nextFFTBlockReady == false)			 // If the block state was not ready
			{
				zeromem(fftData, sizeof(fftData));	 // Reset the fftData array to all 0.
				memcpy(fftData, fifo, sizeof(fifo)); // Copy the fifo array to the fftdata array.
				nextFFTBlockReady = true;			 // Set the blocks state to ready.
			}

			fifoIndex = 0;	// Set the fifo index to 0 so that it can be refilled.
		}

		fifo[fifoIndex++] = sample;	// Add current sample to the end of the fifo array.
	}

    //==============================================================================
    void paint (Graphics& g) override
    {
    }

    void resized() override
    {
		visualiser.setBounds(getBounds().reduced(50));
		filePlayer.setBounds(10, 10, 400, 30);
    }

	void timerCallback() override
	{
		if (filePlayer.getPlaybackState() == true)	// If audio is being played.
		{
			fastFourierTransform.performFrequencyOnlyForwardTransform(fftData);	// Convert fftdata array to spectral components.

			int currentScaledBand = 0;	// Reset current band.

			for (int FFTband = 0; FFTband < FFTSize * 0.5; FFTband++)	// For each relevent band of fftdata.
			{
				if (FFTband >= logarithmicBandThreshold[currentScaledBand]) // If the added fftdata reaches the current bands logarithmic threshold.
				{
					StatAccumulator.addValue(fftData[FFTband]);	// Add the last value of the fftdata block to a Statistics Accumulator.
					// Convert the accumulated frequency data to an average and set as a modifier for the visualiser.
					visualiser.SetZModifier(currentScaledBand, StatAccumulator.getAverage());
					StatAccumulator.reset();	// Clear the old accumulated data.
					currentScaledBand++;		// Increment the current band.
				}
				else
				{
					StatAccumulator.addValue(fftData[FFTband]);	// Add the current value of the fftdata block to a Statistics Accumulator.
				}
			}

			nextFFTBlockReady = false;	// FFT block has been processed and is therefore no longer ready to process.
		}
	}

	void convertBandsToLogScale()
	{
		logarithmicBandThreshold.clear();	// Clear array.

		for (int BandNum = 0; BandNum < visualiserBandsTotal; BandNum++)	// For each band to be drawn.
		{	// Logarithmically convert the number of FFTbands to the number of bands which will be drawn.
			logarithmicBandThreshold.add(1 * pow(((FFTSize * 0.5 - (FFTSize * 0.40))) / 1, BandNum / (visualiserBandsTotal - 1.0)));
			DBG((String)logarithmicBandThreshold.getLast());
		}
	}

private:
    //==============================================================================

	AudioVisualiser visualiser;
	FilePlayer filePlayer;

	FFT fastFourierTransform;

	int visualiserBandsTotal = VisualiserBands;

	enum { FFTOrder = 10, FFTSize = 1 << FFTOrder };
	float fifo[FFTSize];
	float fftData[FFTSize * 2];
	int fifoIndex;
	bool nextFFTBlockReady;

	Array<int> logarithmicBandThreshold;
	StatisticsAccumulator<float> StatAccumulator;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }
