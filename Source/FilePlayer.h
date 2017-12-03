#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FilePlayer : public AudioSource,
	public Component,
	public Button::Listener,
	public FilenameComponentListener,
	public Slider::Listener,
	public Timer
{

public:
/** Constructor */
	FilePlayer();

/** Destructor */
	~FilePlayer();
	
	/** GUI Functions*/

/** Responds to button clicks such as the play button */
	void buttonClicked(Button* button) override;

/** Responds to slider changes such as the playback position */
	void sliderValueChanged(Slider* slider) override;

/** Responds to periodic calls, used to update the playback positon slider */
	void timerCallback() override;

/** Called when the component is resized. */
	void resized() override;

	/** Audio Functions*/

/** Initialises the resampling audio sources parameters.*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override;

/** Processes the audio data (Resampling Audio Source)*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

/** Releases the resampling audio sources resources. */
	void releaseResources()override;

/** Loads and stores an audio file into an audio transport source.*/
	void loadAudioFile(File& file);

/** Calls loadAudioFile when the filechooser chooses a new file.*/
	void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

	/** Getters & Setters */

/** Sets the playback state. */
	void setPlayBackState(bool state);

/** Gets the playback state. */
	bool getPlaybackState();

/** Sets the Sample Rate. */
	void setSampleRate(int sampleRate);

/** Gets the Sample Rate. */
	int getSampleRate();

private:

	// AUDIO VARIABLES/OBJECTS
	AudioFormatReaderSource* audioFormatReaderSource;
	AudioTransportSource audioTransportSource;
	ResamplingAudioSource* resamplingAudioSource;
	TimeSliceThread thread;
	float playBackPosition = 0.0;
	bool playbackState = false;
	int SampleRate = 44100;

	// GUI VARIABLES/OBJECTS
	enum { playButtonID, stopButtonID, numOfButtons };
	TextButton GUIButtons[numOfButtons];
	Slider playbackPositionSlider;
	ScopedPointer<FilenameComponent> fileChooser;
};