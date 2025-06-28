#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Camera;
class Heart :
	public ItemBase
{
public:
	Heart(int modelHandle, Vector3 pos);
	virtual ~Heart();
	//����������
	void Init()override;
	//�X�V����
	void Update(const std::weak_ptr<Camera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//�Փ˃C�x���g
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//�`��
	void Draw()const override;
	//�X�V�����̊m��
	void Complete() override;
private:
	//�ŏ��̐��t���[���͓����蔻����������Ȃ��̂�
	int m_noHitFrame;
};

