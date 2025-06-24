#pragma once
/// <summary>
/// UI‚ÌŠî’êƒNƒ‰ƒX
/// </summary>
class UIBase abstract
{
public:
	UIBase();
	~UIBase();
	virtual void Update() abstract;
	virtual void Draw()const abstract;
};

