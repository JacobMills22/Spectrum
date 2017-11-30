#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class SimpleEnvelope
{
	public:

	private:

	enum {Stopped, Attack, Release};
	int envelopestate = Stopped;
};