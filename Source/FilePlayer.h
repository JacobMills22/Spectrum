#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FilePlayer : public AudioSource,
	public Component,
	public Button::Listener,
	public FilenameComponentListener
{

public:

	FilePlayer();

	~FilePlayer();


	void buttonClicked(Button* button)override;
	void resized() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate)override;
	void releaseResources()override;
	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
	void loadAudioFile(File& file);
	void filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged) override;

	void setPlayBackState(bool state);
	bool getPlaybackState();
	void setSampleRate(int sampleRate);
	int getSampleRate();

private:

	// AUDIO VARIABLES/OBJECTS
	AudioFormatReaderSource* audioFormatReaderSource;
	AudioTransportSource audioTransportSource;
	ResamplingAudioSource* resamplingAudioSource;
	TimeSliceThread thread;
	float playBackPosition = 0.0;
	bool PlaybackState = false;
	int SampleRate = 48000;

	// GUI VARIABLES/OBJECTS
	enum { playButtonID, numOfButtons };
	TextButton GUIButtons[numOfButtons];
	ScopedPointer<FilenameComponent> fileChooser;
};