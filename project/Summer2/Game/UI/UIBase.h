#pragma once
class UIBase abstract
{
public:
	UIBase();
	virtual~UIBase();
	virtual void Init() abstract;
	virtual void Update()abstract;
	virtual void Draw()const abstract;
	virtual void End()abstract;
};

