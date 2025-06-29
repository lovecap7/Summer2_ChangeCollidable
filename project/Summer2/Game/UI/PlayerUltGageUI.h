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
	//�v���C���[�̕K�E�Q�[�W
	std::weak_ptr<UltGage> m_ultGage;
	//�Q�[�W
	float m_viewUltGage;
	float m_viewMaxUltGage;
};
