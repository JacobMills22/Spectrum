#pragma once

/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Visualiser.h"
#include "FilePlayer.h"

#define VisualiserBands 64

/** This is the projects main class which holds the fileplayer and visualiser.
This class is responsible for various GUI components aswell as
performing FFT analysis on audio data supplied by the file player
and transporting it to the visualiser.
*/
class MainContentComponent : public AudioAppComponent,
	public Timer,
	public Button::Listener,
	public ComboBox::Listener,
	public Slider::Listener,
	public ChangeListener
{
public:
	//==============================================================================

	/** Constructor, intialises all components and starts the timer. */
	MainContentComponent();

	/** Destructor, stops timer and shuts down audio. */
	~MainContentComponent();

	//==============================================================================

	/** Prepares the file player for playback*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/** Handles audio playback, pushes audio samples into an array for FFT analysis.
		Also calls "calculateAudioIntensity". */
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

	/** Releases the file players resources. */
	void releaseResources() override;

	/** Puts a sample into an array ready for FFT analysis */
	void pushNextSampleIntoFifo(float sample) noexcept;

	/** Gets number of bands to be rendered, and logarithmically scales each bands bandwidth. */
	void convertBandsToLogScale();

	/** Calculates the intensity of the incoming audio and sets the value to "fileplayerAmplitude".
	    This variable is then used to conytrol the rotation speed if auto rotate is enabled.
		This should be called for each sample in a buffer.
		@param amplitude: is the sum of the current left and right samples. */
	void calculateAudioIntensity(float amplitude);

	//==============================================================================

	/** Sets bounds of all components whenever the window is resized. */ 
	void resized() override;

	/** Responds to various button presses such as rotation toggles. */
	void buttonClicked(Button* button) override;

	/** Responds to visualisation preset combobox changes. */
	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;

	/** Responds to slider changes such as rotation sliders. */
	void sliderValueChanged(Slider* sliderchanged) override;

	/** Responds to change listener callbacks such as colour selectors. */
	void changeListenerCallback(ChangeBroadcaster* source) override;

	/** Called every 30ms and updates auto rotation speed, performs FFT analysis and 
	sets spectrum data to the visualisations. */
	void timerCallback() override;
	
	//==============================================================================

	/** Sets edit state to @param state. */
	void setEditState(bool state);

	/** Returns the current edit state. */
	bool getEditState();

private:
	//==============================================================================

	AudioVisualiser visualiser;
	FilePlayer filePlayer;

	TextButton editButton;
	bool editState = false;

	ComboBox visualisationPresetBox;

	enum { rotationXSliderID, rotationYSliderID, rotationSpeedSliderID, numOfBandsSliderID, decaySpeedSliderID, numOfSliders };
	Slider slider[numOfSliders];

	enum { rotationSpeedLabelID, numOfBandsLabelID, decaySpeedLabelID, numOfLabels };
	Label labels[numOfLabels];

	enum { rotateXToggleID, rotateYToggleID, rotateAutoToggleID, numOfToggleButtons };
	ToggleButton rotateToggle[numOfToggleButtons];

	ColourSelector visualisationColourSelector[2];

	FFT fastFourierTransform;
	enum { FFTOrder = 10, FFTSize = 1 << FFTOrder };
	float fifo[FFTSize];
	float fftData[FFTSize * 2];
	int fifoIndex;
	bool nextFFTBlockReady;
	Array<int> logarithmicBandThreshold;
	StatisticsAccumulator<float> StatAccumulator;

	float fileplayerAmplitude = 0.0;
	float rotationSpeedScale = 1.0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};

// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }
