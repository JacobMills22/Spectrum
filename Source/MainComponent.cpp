
#include "MainComponent.h"

// Older versions of FFT need to be constructed with "fastFourierTransform(FFTOrder, false)".
MainContentComponent::MainContentComponent() : fastFourierTransform(FFTOrder),
visualiser(VisualiserBands)
{
    setSize (1100, 700);
    setAudioChannels(0, 2);
    filePlayer.setLevel(0.00);
    
    // Visualiser initialisation
    addAndMakeVisible(visualiser);
    visualiser.setSize(800, 600);
    addAndMakeVisible(filePlayer);
    fifoIndex = 0;
    nextFFTBlockReady = false;
    convertBandsToLogScale();
    
    // Edit Button initialisation
    addAndMakeVisible(editButton);
    editButton.setButtonText("Edit");
    editButton.addListener(this);
    
    // Preset ComboBox initialisation
    addAndMakeVisible(visualisationPresetBox);
    visualisationPresetBox.addListener(this);
    visualisationPresetBox.setVisible(false);
    visualisationPresetBox.addItem("Spectral Cube", visualiser.spectralCubeID);
    visualisationPresetBox.addItem("Droplet", visualiser.dropletID);
    visualisationPresetBox.setSelectedId(visualiser.spectralCubeID);
    
    // Slider initialisation
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
    slider[rotationSpeedSliderID].setValue(1.0);
    
    slider[numOfBandsSliderID].setTextBoxStyle(Slider::TextBoxRight, true, 50, 20);
    slider[numOfBandsSliderID].setRange(8, 128, 1);
    slider[numOfBandsSliderID].setValue(VisualiserBands);
    
    slider[decaySpeedSliderID].setTextBoxStyle(Slider::TextBoxRight, true, 50, 20);
    slider[decaySpeedSliderID].setRange(0, 20, 1);
    slider[decaySpeedSliderID].setValue(10);
    
    // Label initialisation
    for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
    {
        addAndMakeVisible(labels[labelNum]);
        labels[labelNum].setVisible(false);
    }
    
    labels[rotationSpeedLabelID].setText("Rotation Speed", dontSendNotification);
    labels[numOfBandsLabelID].setText("Number of Bands", dontSendNotification);
    labels[decaySpeedLabelID].setText("Decay Speed", dontSendNotification);
    
    // Toggle buttons initialisation
    for (int toggleButtonID = 0; toggleButtonID < numOfToggleButtons; toggleButtonID++)
    {
        addAndMakeVisible(rotateToggle[toggleButtonID]);
        rotateToggle[toggleButtonID].addListener(this);
        rotateToggle[toggleButtonID].setToggleState(false, dontSendNotification);
        rotateToggle[toggleButtonID].setVisible(false);
    }
    
    rotateToggle[rotateXToggleID].setButtonText("Rotate Veritcle");
    rotateToggle[rotateYToggleID].setButtonText("Rotate Horizontal");
    rotateToggle[rotateAutoToggleID].setButtonText("Auto Rotate");
    
    // Colour selector initialisation
    addAndMakeVisible(visualisationColourSelector[0]);
    visualisationColourSelector[0].addChangeListener(this);
    visualisationColourSelector[0].setVisible(false);
    visualiser.setRenderColour(0, 0.0, 0.5, 1.0);
    
    addAndMakeVisible(visualisationColourSelector[1]);
    visualisationColourSelector[1].addChangeListener(this);
    visualisationColourSelector[1].setVisible(false);
    visualiser.setRenderColour(1, 0.0, 0.05, 0.05);
    
    // Start timer
    startTimer(30);
}

	MainContentComponent::~MainContentComponent()
    {
		shutdownAudio();
		stopTimer();
    }

    //==============================================================================
    void MainContentComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
    {
		filePlayer.prepareToPlay(samplesPerBlockExpected, sampleRate); // Tell the filePlayer the samplerate and how many samples to expect.
    }

    void MainContentComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) 
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

			if (rotateToggle[rotateAutoToggleID].getToggleState() == true)
			{
				calculateAudioIntensity(outputL[sample] + outputR[sample]);
			}

			outputL[sample] *= filePlayer.getLevel();	// Scale audio down.
			outputR[sample] *= filePlayer.getLevel();	// Scale audio down.
		}
	}

    void MainContentComponent::releaseResources() 
    {
		filePlayer.releaseResources();
    }

	void MainContentComponent::pushNextSampleIntoFifo(float sample) noexcept
	{
		// Once Fifo has enough data, set the block state to ready.
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

	void MainContentComponent::convertBandsToLogScale()
	{
		logarithmicBandThreshold.clear();	// Clear array.

		for (int BandNum = 0; BandNum < visualiser.getNumOfBandsToRender(); BandNum++)	// For each band to be drawn.
		{	// Logarithmically convert the number of FFTbands to the number of bands which will be drawn.
			logarithmicBandThreshold.add(1 * pow(((FFTSize * 0.5 - (FFTSize * 0.40))) / 1, BandNum / (visualiser.getNumOfBandsToRender() - 1.0)));
		}
	}

	void MainContentComponent::calculateAudioIntensity(float amplitude)
	{
		float rotationDecay = 0.01;
		amplitude *= rotationSpeedScale * 2;

		if (amplitude <= 1.0 && amplitude >= 0.0)
		{
			if (amplitude < fileplayerAmplitude)
			{
				if (amplitude > fileplayerAmplitude - rotationDecay)
				{
					fileplayerAmplitude = amplitude;
				}
				else
				{
					fileplayerAmplitude -= rotationDecay;
				}
			}
			else if (amplitude > fileplayerAmplitude)
			{
				if (amplitude < fileplayerAmplitude + rotationDecay)
				{
					fileplayerAmplitude = amplitude;
				}
				else
				{
					fileplayerAmplitude += rotationDecay;
				}
			}
			else
			{
				fileplayerAmplitude = amplitude;
			}
		}
	}

    //==============================================================================

    void MainContentComponent::resized() 
    {
		if (getEditState() == true) { visualiser.setBounds(getBounds().reduced(50).removeFromLeft(getBounds().reduced(50).getWidth() * 0.75)); }
		else { visualiser.setBounds(getBounds().reduced(50)); }
		filePlayer.setBounds(visualiser.getBounds().getX(), visualiser.getBounds().getY() - 40, visualiser.getBounds().getWidth(), 30);
		editButton.setBounds(visualiser.getBounds().getWidth(), visualiser.getBounds().getHeight() + 60, 50, 20);

		float visualiserWidth = visualiser.getWidth();
		float editPanelWidth = getBounds().getWidth() - visualiser.getBounds().getRight() - 10;

		visualisationPresetBox.setBounds(			visualiserWidth + 60, filePlayer.getBounds().getHeight() + 20,    150,            25);
		rotateToggle[rotateAutoToggleID].setBounds(visualiserWidth + 60, visualisationPresetBox.getBottom() + 20, editPanelWidth, 30);
		rotateToggle[rotateXToggleID].setBounds(    visualiserWidth + 60, rotateToggle[rotateAutoToggleID].getBottom() + 20,    editPanelWidth, 30);
		slider[rotationXSliderID].setBounds(		visualiserWidth + 60, rotateToggle[rotateXToggleID].getBottom(),  editPanelWidth, 30);
		rotateToggle[rotateYToggleID].setBounds(	visualiserWidth + 60, slider[rotationXSliderID].getBottom() + 20, editPanelWidth, 30);
		slider[rotationYSliderID].setBounds(		visualiserWidth + 60, rotateToggle[rotateYToggleID].getBottom(),  editPanelWidth, 30);

		labels[rotationSpeedLabelID].setBounds(		visualiserWidth + 60, slider[rotationYSliderID].getBottom() + 20, editPanelWidth, 30);
		slider[rotationSpeedSliderID].setBounds(	visualiserWidth + 60, labels[rotationSpeedLabelID].getBottom(),   editPanelWidth, 30);

		labels[numOfBandsLabelID].setBounds(		visualiserWidth + 60, slider[rotationSpeedSliderID].getBottom(), editPanelWidth, 30);
		slider[numOfBandsSliderID].setBounds(		visualiserWidth + 60, labels[numOfBandsLabelID].getBottom(), editPanelWidth - 5, 30);

		labels[decaySpeedLabelID].setBounds(		visualiserWidth + 60, slider[numOfBandsSliderID].getBottom(), editPanelWidth, 30);
		slider[decaySpeedSliderID].setBounds(		visualiserWidth + 60, labels[decaySpeedLabelID].getBottom(), editPanelWidth - 5, 30);

		visualisationColourSelector[0].setBounds(	visualiserWidth + 60, slider[decaySpeedSliderID].getBottom() + 10, editPanelWidth * 0.5 - 10, 200);
		visualisationColourSelector[1].setBounds(	visualiserWidth + visualisationColourSelector[0].getWidth() + 70, slider[decaySpeedSliderID].getBottom() + 10, editPanelWidth * 0.5 - 10, 200);

    }

	void MainContentComponent::buttonClicked(Button* button) 
	{
		if (button == &editButton)		// If edit button was pressed.
		{
			if (getEditState() == true)	// If edit state was true make it false and
			{							// make all edit parameters invisible.
				setEditState(false);
				visualisationPresetBox.setVisible(false);
				rotateToggle[rotateXToggleID].setVisible(false);
				rotateToggle[rotateYToggleID].setVisible(false);
				rotateToggle[rotateAutoToggleID].setVisible(false);
				visualisationColourSelector[0].setVisible(false);
				visualisationColourSelector[1].setVisible(false);

				for (int sliderID = 0; sliderID < numOfSliders; sliderID++)
					slider[sliderID].setVisible(false);

				for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
					labels[labelNum].setVisible(false);

			}
			else if (getEditState() == false) // If edit state was false make it true and
			{								  // make all edit parameters visible.
				setEditState(true);
				visualisationPresetBox.setVisible(true);
				rotateToggle[rotateXToggleID].setVisible(true);
				rotateToggle[rotateYToggleID].setVisible(true);
				rotateToggle[rotateAutoToggleID].setVisible(true);
				visualisationColourSelector[0].setVisible(true);
				visualisationColourSelector[1].setVisible(true);

				for (int sliderID = 0; sliderID < numOfSliders; sliderID++)
					slider[sliderID].setVisible(true);

				for (int labelNum = 0; labelNum < numOfLabels; labelNum++)
					labels[labelNum].setVisible(true);
			}

			resized();
		}
		else if (button == &rotateToggle[rotateXToggleID]) // If Rotate X button was pressed set the
		{												   // rotating state to the new state of the button
			visualiser.setRotatingXState(rotateToggle[rotateXToggleID].getToggleState());
		}
		else if (button == &rotateToggle[rotateYToggleID]) // If Rotate Y button was pressed set the
		{												   // rotating state to the new state of the button
			visualiser.setRotatingYState(rotateToggle[rotateYToggleID].getToggleState());
		}
		else if (button == &rotateToggle[rotateAutoToggleID])
		{
			if (rotateToggle[rotateAutoToggleID].getToggleState() == true)
			{
				labels[rotationSpeedLabelID].setText("Rotation Speed Scale", dontSendNotification);
			}
			else if (rotateToggle[rotateAutoToggleID].getToggleState() == false)
			{
				labels[rotationSpeedLabelID].setText("Rotation Speed", dontSendNotification);
				visualiser.setRotationSpeed(slider[rotationSpeedSliderID].getValue());
			}
		}
	}

	void MainContentComponent::comboBoxChanged(ComboBox* comboBoxThatHasChanged) 
	{
		if (comboBoxThatHasChanged == &visualisationPresetBox) // If the selected preset was changed
		{	
			// Set the visualisation to draw to the selected ID
			visualiser.setVisualisationToDraw(visualisationPresetBox.getSelectedId());	
			
			// Reset the rotation values to a standard default.
			slider[rotationXSliderID].setMaxValue(5);
			slider[rotationXSliderID].setMinValue(5);
			slider[rotationYSliderID].setMaxValue(100);
			slider[rotationYSliderID].setMinValue(100);
		}
	}

	void MainContentComponent::sliderValueChanged(Slider* sliderchanged) 
	{
		// Set relevent paramters to the corresponding sliders current value.
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
			if (rotateToggle[rotateAutoToggleID].getToggleState() == true)
			{
				rotationSpeedScale = slider[rotationSpeedSliderID].getValue();
			}
			else
			{
				visualiser.setRotationSpeed(slider[rotationSpeedSliderID].getValue());
			}
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

	void MainContentComponent::changeListenerCallback(ChangeBroadcaster* source) 
	{
		// Set the colour of a specified element of the render.
		if (source == &visualisationColourSelector[0]) 
		{
			visualiser.setRenderColour(0, visualisationColourSelector[0].getCurrentColour().getFloatRed(),
										  visualisationColourSelector[0].getCurrentColour().getFloatGreen(),
										  visualisationColourSelector[0].getCurrentColour().getFloatBlue());
		}
		else if (source == &visualisationColourSelector[1])
		{
			visualiser.setRenderColour(1, visualisationColourSelector[1].getCurrentColour().getFloatRed(),
										  visualisationColourSelector[1].getCurrentColour().getFloatGreen(),
										  visualisationColourSelector[1].getCurrentColour().getFloatBlue());
		}
	}


	void MainContentComponent::timerCallback() 
	{
		if (filePlayer.getPlaybackState() == true)	// If audio is being played.
		{
			if (rotateToggle[rotateAutoToggleID].getToggleState() == true)
			{
				visualiser.setRotationSpeed(fileplayerAmplitude);
			}

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

	//==============================================================================

	void MainContentComponent::setEditState(bool state)
	{
		editState = state;
	}

	bool MainContentComponent::getEditState()
	{
		return editState;
	}

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }
