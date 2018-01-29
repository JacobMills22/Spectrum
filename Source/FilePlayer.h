#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


/** Simple file player class which handles all of
the audio playback of the project, users load an audio file
into this class which is streamed using a ResamplingAudioSource.
Includes file selector, volume, track location and play, pause and stop buttons. 
*/
class FilePlayer : public AudioSource,
	public Component,
	public Button::Listener,
	public FilenameComponentListener,
	public Slider::Listener,
	public Timer
{

public:
/** Constructor, handles both audio and GUI initialisation. */
	FilePlayer();

/** Destructor, stops the timer and audio thread and deletes/deallocates the
	audioFormatReaderSource, resamplingAudioSource and audioTransportSource. */
	~FilePlayer();
	
/** Responds to button clicks such as the play button */
	void buttonClicked(Button* button) override;

/** Responds to slider changes such as the playback position */
	void sliderValueChanged(Slider* slider) override;

/** Responds to periodic calls, used to update the playback positon slider */
	void timerCallback() override;

/** Called when the component is resized and updates the bounds of all components */
	void resized() override;

/** Initialises the resampling audio sources parameters.*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override;

/** Processes the audio data (Resampling Audio Source)*/
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;

/** Releases the resampling audio sources resources. */
	void releaseResources()override;

/** Loads and stores an audio file into an audio transport source.
	@param file: The audio file to load. */
	void loadAudioFile(File& file);

/** Calls loadAudioFile when the filechooser chooses a new file.*/
	void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

/** Sets the playback state. */
	void setPlayBackState(bool state);

/** Gets the playback state. */
	bool getPlaybackState();

/** Sets the Sample Rate. */
	void setSampleRate(int sampleRate);

/** Gets the Sample Rate. */
	int getSampleRate();

/** Set the playback level modifier */
	void setLevel(float level);

/** Get the playback level modifier */
	float getLevel();

private:

	// AUDIO VARIABLES/OBJECTS
	AudioFormatReaderSource* audioFormatReaderSource;
	AudioTransportSource audioTransportSource;
	ResamplingAudioSource* resamplingAudioSource;
	TimeSliceThread thread;
	float playBackPosition = 0.0;
	bool playbackState = false;
	int SampleRate = 44100;
	float playbackLevel = 0.0;

	// GUI VARIABLES/OBJECTS
	enum { playButtonID, stopButtonID, numOfButtons };
	TextButton GUIButtons[numOfButtons];
	enum { playbackPositionSliderID, levelSliderID, numOfSliderIDs};
	Slider fileplayerSlider[numOfSliderIDs];
	ScopedPointer<FilenameComponent> fileChooser;
};