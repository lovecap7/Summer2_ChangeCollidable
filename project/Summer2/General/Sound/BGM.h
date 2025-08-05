#pragma once
#include "SoundBase.h"
class BGM :
    public SoundBase
{
public:
	BGM(int handle, int volume, bool isLoop);
	~BGM();
	void ChangeBGM(int handle);
};

