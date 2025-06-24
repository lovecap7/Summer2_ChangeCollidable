#pragma once
#include "UIBase.h"
#include <memory>
class UltGage;
class UIPlayerUltGage :
    public UIBase
{
public:
	UIPlayerUltGage(std::shared_ptr<UltGage> ultGage);
	~UIPlayerUltGage();
	void Update() override;
	void Draw()const override;
private:
	std::shared_ptr<UltGage> m_ultGage;
	float m_nowUltGage;
	float m_maxUltGage;
};

