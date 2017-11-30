#include "FilePlayer.h"


FilePlayer::FilePlayer() : thread("AudioFileStreamThread")
{
	//Audio Initialisation
	thread.startThread();
	audioFormatReaderSource = NULL;
	resamplingAudioSource = new ResamplingAudioSource(&audioTransportSource, false);

	//GUI Initialisation
	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();
	fileChooser = new FilenameComponent("AudioFile", File::nonexistent, true, false, false,
		formatManager.getWildcardForAllFormats(), String::empty, "Choose an audio file");

	fileChooser->addListener(this);
	addAndMakeVisible(fileChooser);

	for (int ButtonID = 0; ButtonID < numOfButtons; ButtonID++)
	{
		addAndMakeVisible(GUIButtons[ButtonID]);
		GUIButtons[ButtonID].addListener(this);
	}
	GUIButtons[playButtonID].setButtonText("Play");
}

FilePlayer::~FilePlayer()
{
	audioTransportSource.setSource(0);
	deleteAndZero(audioFormatReaderSource);
	thread.stopThread(100);
	delete resamplingAudioSource;
	delete fileChooser;
}

// GUI

void FilePlayer::buttonClicked(Button* button)
{
	if (button == &GUIButtons[playButtonID])	// If Play button was clicked,
	{											// Start/Stop audio Playback.
		setPlayBackState(!getPlaybackState());
		if (getPlaybackState() == true)
		{
			audioTransportSource.start();
			GUIButtons[playButtonID].setButtonText("Stop");
		}
		else
		{
			audioTransportSource.stop();
			audioTransportSource.setPosition(0);
			GUIButtons[playButtonID].setButtonText("Play");
		}
	}
}

void FilePlayer::resized()
{
	// Set bounds of components.
	GUIButtons[playButtonID].setBounds(270, 0, 100, 30);
	fileChooser->setBounds(0, 0, 250, 30);
}

// AUDIO

void FilePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	resamplingAudioSource->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void FilePlayer::releaseResources()
{
	resamplingAudioSource->releaseResources();
}

void FilePlayer::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
	resamplingAudioSource->getNextAudioBlock(bufferToFill);
}

void FilePlayer::loadAudioFile(File& file)
{										// File is loading so
	setPlayBackState(false);			// Stop audio playback
	audioTransportSource.setSource(0);  // Remove the source from the audio transport source.
	deleteAndZero(audioFormatReaderSource); // Remove audioformatreadersource

	AudioFormatManager formatManager;
	formatManager.registerBasicFormats();	// Adds basic audio formats

	AudioFormatReader* reader = formatManager.createReaderFor(file); // Creates a reader for the audio file.

	if (reader != 0)	// If the reader has a file to read
	{
		audioFormatReaderSource = new AudioFormatReaderSource(reader, true);	// Create a AudioFormatReaderSource

		// Set the audioTransportSource to be ready to play the audio file on the audio thread.
		audioTransportSource.setSource(audioFormatReaderSource, getSampleRate(), &thread, reader->sampleRate);
	}
}

void FilePlayer::filenameComponentChanged(FilenameComponent* fileComponentThatHasChanged)
{
	if (fileComponentThatHasChanged == fileChooser)
	{
		File audioFile(fileChooser->getCurrentFile().getFullPathName());

		if (audioFile.existsAsFile())
		{
			loadAudioFile(audioFile);
		}
	}
}


// GETTERS & SETTERS

void FilePlayer::setPlayBackState(bool state)
{
	if (state == true)
	{
		audioTransportSource.setPosition(0);
		audioTransportSource.start();
	}
	else
	{
		audioTransportSource.stop();
	}
}

bool FilePlayer::getPlaybackState()
{
	return audioTransportSource.isPlaying();
}

void FilePlayer::setSampleRate(int sampleRate)
{
	SampleRate = sampleRate;
}

int FilePlayer::getSampleRate()
{
	return SampleRate;
}