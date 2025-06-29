#pragma once
#include "UIBase.h"
#include <memory>
class UltGage;
class PlayerUltGageUI :
	public UIBase
{
public:
	PlayerUltGageUI(std::weak_ptr<UltGage> ultGage);
	~PlayerUltGageUI();
	void Init() override;
	void Update()override;
	void Draw()const override;
	void End()override;
private:
	//プレイヤーの必殺ゲージ
	std::weak_ptr<UltGage> m_ultGage;
	//ゲージ
	float m_viewUltGage;
	float m_viewMaxUltGage;
};
