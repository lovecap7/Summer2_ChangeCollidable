#pragma once
#include "UIBase.h"
#include <memory>
class HitPoints;
class PlayerHpUI :
    public UIBase
{
public:
	PlayerHpUI(std::weak_ptr<HitPoints> hp);
	~PlayerHpUI();
	void Init() override;
	void Update()override;
	void Draw()const override;
	void End()override;
private:
	//�v���C���[�̗̑�
	std::weak_ptr<HitPoints> m_hitPoints;
	//�̗�
	float m_viewHp;
	float m_viewMaxHp;

};

