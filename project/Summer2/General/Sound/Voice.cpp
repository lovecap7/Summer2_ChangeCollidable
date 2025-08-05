#include "Voice.h"

Voice::Voice(int handle, int volume):
	SoundBase(handle,volume,false)
{
}

Voice::~Voice()
{
}
