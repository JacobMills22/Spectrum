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
	GUIButtons[stopButtonID].setButtonText("Stop");
	
	addAndMakeVisible(playbackPositionSlider);
	playbackPositionSlider.setRange(0.0, 1.0, 0.01);
	playbackPositionSlider.setTextBoxStyle(Slider::TextBoxRight, false, 50, 15);
	playbackPositionSlider.addListener(this);

	startTimer(100);
}

FilePlayer::~FilePlayer()
{
	stopTimer();
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
	{											// Start/Pause audio Playback.
		if (getPlaybackState() == false)
		{
			audioTransportSource.setPosition(playBackPosition);
			setPlayBackState(true);
			GUIButtons[playButtonID].setButtonText("Pause");
		}
		else
		{
			setPlayBackState(false);
			GUIButtons[playButtonID].setButtonText("Play");
		}
	}
	else if (button == &GUIButtons[stopButtonID])	// If Stop button was clicked,
	{
		setPlayBackState(false);
		playBackPosition = 0;
		audioTransportSource.setPosition(playBackPosition);
		GUIButtons[playButtonID].setButtonText("Play");
	}
}

void FilePlayer::sliderValueChanged(Slider* slider)
{
	if (slider == &playbackPositionSlider)
	{
		playBackPosition = playbackPositionSlider.getValue();

		if (slider->isMouseButtonDown() == true)
		{
			setPlayBackState(false);
			GUIButtons[playButtonID].setButtonText("Play");
		}
	}
}

void FilePlayer::timerCallback()
{
	if (getPlaybackState() == true)
	{
		playbackPositionSlider.setValue(audioTransportSource.getCurrentPosition());
		playBackPosition = audioTransportSource.getCurrentPosition();
	}
}

void FilePlayer::resized()
{
	// Set bounds of components.
	fileChooser->setBounds			  (0, 0, getWidth() * 0.3, 30);
	GUIButtons[playButtonID].setBounds(getWidth() * 0.31, 0, getWidth() * 0.075, 30);
	GUIButtons[stopButtonID].setBounds(getWidth() * 0.395, 0, getWidth() * 0.075, 30);
	playbackPositionSlider.setBounds  (getWidth() * 0.48, 0, getWidth() * 0.52, 30);
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

	playbackPositionSlider.setRange(0.0, audioTransportSource.getLengthInSeconds(), 0.01);
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
