#pragma once
#include "CapsuleAttackBase.h"
class ActorStateBase;
class Lariat :
	public CapsuleAttackBase
{
public:
	Lariat(std::weak_ptr<ActorStateBase> ownerState, int& modelHandle, int startIndex, int endIndex, float radius);
	~Lariat();
	//����������
	void Init()override;
	//�X�V����
	virtual void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�`��
	void Draw()const override;
private:

};

