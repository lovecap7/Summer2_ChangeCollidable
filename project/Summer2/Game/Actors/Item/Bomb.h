#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Camera;
class Bomb :
	public ItemBase
{
public:
	Bomb(int modelHandle, Vector3 pos);
	virtual ~Bomb();
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
	//���S����
	void Dead(const std::weak_ptr<ActorManager> actorManager) override;
private:
	//�����̃J�E���g
	int m_blastCountFrame;
	//���f���̃T�C�Y
	Vector3 m_originalScale;//���̃��f���̃T�C�Y��ێ�
	//���f���̑傫����cos���g���ĕύX����̂Ŋp�x��ێ�
	float m_scaleSpeed;
};
