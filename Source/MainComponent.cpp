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
		                       public Timer,
							   public Button::Listener,
							   public ComboBox::Listener,
	                           public Slider::Listener
{
public:
    //==============================================================================
    MainContentComponent() : fastFourierTransform(FFTOrder, false),
							 visualiser(VisualiserBands)
    {
        setSize (800, 600);
		setAudioChannels(0, 2);
		filePlayer.setLevel(0.00);

		addAndMakeVisible(visualiser);
		visualiser.setSize(800, 600);
		addAndMakeVisible(filePlayer);
		fifoIndex = 0;
		nextFFTBlockReady = false;

		convertBandsToLogScale();

		addAndMakeVisible(editButton);
		editButton.setButtonText("Edit");
		editButton.addListener(this);

		addAndMakeVisible(visualisationPresetBox);
		visualisationPresetBox.addListener(this);
		visualisationPresetBox.setVisible(false);
		visualisationPresetBox.addItem("Spectral Cube", visualiser.spectralCubeID);
		visualisationPresetBox.addItem("Droplet", visualiser.dropletID);
		visualisationPresetBox.setSelectedId(visualiser.spectralCubeID);


		for (int sliderID = 0; sliderID < numOfSliders; sliderID++)
		{
			addAndMakeVisible(slider[sliderID]);
			slider[sliderID].setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
			slider[sliderID].setRange(0, 360, 1);
			slider[sliderID].addListener(this);
			slider[sliderID].setVisible(false);
		}

		slider[rotateXToggleID].setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);
		slider[rotateYToggleID].setSliderStyle(Slider::SliderStyle::TwoValueHorizontal);

		slider[rotationSpeedSliderID].setRange(0.0, 2.0, 0.1);

		slider[numOfBandsSliderID].setTextBoxStyle(Slider::TextBoxRight, true, 50, 20);
		slider[numOfBandsSliderID].setRange(8, 128, 1);
		slider[numOfBandsSliderID].setValue(VisualiserBands);

		slider[decaySpeedSliderID].setTextBoxStyle(Slider::TextBoxRight, true, 50, 20);
		slider[decaySpeedSliderID].setRange(0, 20, 1);
		slider[decaySpeedSliderID].setValue(10);


		for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
		{
			addAndMakeVisible(labels[labelNum]);
			labels[labelNum].setVisible(false);
		}

		labels[rotationSpeedLabelID].setText("Rotation Speed", dontSendNotification);
		labels[numOfBandsLabelID].setText("Number of Bands", dontSendNotification);
		labels[decaySpeedLabelID].setText("Decay Speed", dontSendNotification);
		
		for (int toggleButtonID = 0; toggleButtonID < numOfToggleButtons; toggleButtonID++)
		{
			addAndMakeVisible(rotateToggle[toggleButtonID]);
			rotateToggle[toggleButtonID].addListener(this);
			rotateToggle[toggleButtonID].setToggleState(false, dontSendNotification);
			rotateToggle[toggleButtonID].setVisible(false);
		}

		rotateToggle[rotateXToggleID].setButtonText("Rotate Veritcle");
		rotateToggle[rotateYToggleID].setButtonText("Rotate Horizontal");

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

		filePlayer.getNextAudioBlock(bufferToFill);	// Fill buffer with the next audio block from filePlayer.

		float *outputL = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample); // Writeable Left Channel Output Array.
		float *outputR = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample); // Writeable Right Channel Output Array.

		for (int sample = 0; sample < bufferToFill.numSamples; sample++) // For each sample.
		{
			if (filePlayer.getPlaybackState() == true) // If the Fileplayer is playing audio.
			{
				pushNextSampleIntoFifo(outputL[sample]); // Add current sample to fifo array.
			}

			outputL[sample] *= filePlayer.getLevel();	// Scale audio down.
			outputR[sample] *= filePlayer.getLevel();	// Scale audio down.
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
		if (getEditState() == true) { visualiser.setBounds(getBounds().reduced(50).removeFromLeft(getBounds().reduced(50).getWidth() * 0.80)); }
		else { visualiser.setBounds(getBounds().reduced(50)); }
		filePlayer.setBounds(visualiser.getBounds().getX(), visualiser.getBounds().getY() - 40, visualiser.getBounds().getWidth(), 30);
		editButton.setBounds(visualiser.getBounds().getWidth(), visualiser.getBounds().getHeight() + 60, 50, 20);

		float visualiserWidth = visualiser.getWidth();
		float editPanelWidth = getBounds().getWidth() - visualiser.getBounds().getRight() - 10;

		visualisationPresetBox.setBounds(       visualiserWidth + 60, filePlayer.getBounds().getHeight() + 20,    150,            25);
		rotateToggle[rotateXToggleID].setBounds(visualiserWidth + 60, visualisationPresetBox.getBottom() + 20,    editPanelWidth, 30);
		slider[rotationXSliderID].setBounds(    visualiserWidth + 60, rotateToggle[rotateXToggleID].getBottom(),  editPanelWidth, 30);
		rotateToggle[rotateYToggleID].setBounds(visualiserWidth + 60, slider[rotationXSliderID].getBottom() + 20, editPanelWidth, 30);
		slider[rotationYSliderID].setBounds(    visualiserWidth + 60, rotateToggle[rotateYToggleID].getBottom(),  editPanelWidth, 30);

		labels[rotationSpeedLabelID].setBounds( visualiserWidth + 60, slider[rotationYSliderID].getBottom() + 20, editPanelWidth, 30);
		slider[rotationSpeedSliderID].setBounds(visualiserWidth + 60, labels[rotationSpeedLabelID].getBottom(),   editPanelWidth, 30);

		labels[numOfBandsLabelID].setBounds(visualiserWidth + 60, slider[rotationSpeedSliderID].getBottom(), editPanelWidth, 30);
		slider[numOfBandsSliderID].setBounds(visualiserWidth + 60, labels[numOfBandsLabelID].getBottom(), editPanelWidth - 5, 30);

		labels[decaySpeedLabelID].setBounds(visualiserWidth + 60, slider[numOfBandsSliderID].getBottom(), editPanelWidth, 30);
		slider[decaySpeedSliderID].setBounds(visualiserWidth + 60, labels[decaySpeedLabelID].getBottom(), editPanelWidth - 5, 30);


    }

	void buttonClicked(Button* button) override
	{
		if (button == &editButton)
		{
			if (getEditState() == true)
			{
				setEditState(false);
				visualisationPresetBox.setVisible(false);
				rotateToggle[rotateXToggleID].setVisible(false);
				rotateToggle[rotateYToggleID].setVisible(false);

				for (int sliderID = 0; sliderID < numOfSliders; sliderID++)
					slider[sliderID].setVisible(false);

				for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
					labels[labelNum].setVisible(false);

			}
			else if (getEditState() == false)
			{
				setEditState(true);
				visualisationPresetBox.setVisible(true);
				rotateToggle[rotateXToggleID].setVisible(true);
				rotateToggle[rotateYToggleID].setVisible(true);

				for (int sliderID = 0; sliderID < numOfSliders; sliderID++)
					slider[sliderID].setVisible(true);

				for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
					labels[labelNum].setVisible(true);
			}
			resized();
		}
		else if (button == &rotateToggle[rotateXToggleID])
		{
			visualiser.setRotatingXState(rotateToggle[rotateXToggleID].getToggleState());
		}
		else if (button == &rotateToggle[rotateYToggleID])
		{
			visualiser.setRotatingYState(rotateToggle[rotateYToggleID].getToggleState());
		}
	}

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
	{
		if (comboBoxThatHasChanged == &visualisationPresetBox)
		{
			visualiser.setVisualisationToDraw(visualisationPresetBox.getSelectedId());	

			slider[rotationXSliderID].setMaxValue(5);
			slider[rotationXSliderID].setMinValue(5);
			slider[rotationYSliderID].setMaxValue(100);
			slider[rotationYSliderID].setMinValue(100);
		}
	}

	void sliderValueChanged(Slider* sliderchanged) override
	{
		if (sliderchanged == &slider[rotationXSliderID])
		{
			visualiser.setHorizontalRotationMin(slider[rotationXSliderID].getMinValue());
			visualiser.setHorizontalRotationMax(slider[rotationXSliderID].getMaxValue());
		}
		else if (sliderchanged == &slider[rotationYSliderID])
		{
			visualiser.setVerticleRotationMin(slider[rotationYSliderID].getMinValue());
			visualiser.setVerticleRotationMax(slider[rotationYSliderID].getMaxValue());
		}
		else if (sliderchanged == &slider[rotationSpeedSliderID])
		{
			visualiser.setRotationSpeed(slider[rotationSpeedSliderID].getValue());
		}
		else if (sliderchanged == &slider[numOfBandsSliderID])
		{
			visualiser.setNumOfBandsToRender(slider[numOfBandsSliderID].getValue());
			convertBandsToLogScale();
		}
		else if (sliderchanged == &slider[decaySpeedSliderID])
		{
			visualiser.setBandDecay(slider[decaySpeedSliderID].getValue());
		}
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
					visualiser.setSpectrumData(currentScaledBand, StatAccumulator.getAverage());
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

		for (int BandNum = 0; BandNum < visualiser.getNumOfBandsToRender(); BandNum++)	// For each band to be drawn.
		{	// Logarithmically convert the number of FFTbands to the number of bands which will be drawn.
			logarithmicBandThreshold.add(1 * pow(((FFTSize * 0.5 - (FFTSize * 0.40))) / 1, BandNum / (visualiser.getNumOfBandsToRender() - 1.0)));
			DBG((String)logarithmicBandThreshold.getLast());
		}
	}


	//==============================================================================

	void setEditState(bool state)
	{
		editState = state;
	}

	bool getEditState()
	{
		return editState;
	}

private:
    //==============================================================================

	AudioVisualiser visualiser;
	FilePlayer filePlayer;

	TextButton editButton;
	bool editState = false;

	ComboBox visualisationPresetBox;

	enum {rotationXSliderID, rotationYSliderID, rotationSpeedSliderID, numOfBandsSliderID, decaySpeedSliderID, numOfSliders};
	Slider slider[numOfSliders];

	enum { rotationSpeedLabelID, numOfBandsLabelID, decaySpeedLabelID, numOfLabels};
	Label labels[numOfLabels];
	
	enum{rotateXToggleID, rotateYToggleID, numOfToggleButtons};
	ToggleButton rotateToggle[numOfToggleButtons];

	FFT fastFourierTransform;
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
