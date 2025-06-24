#pragma once
#include "ItemBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>

class ActorManager;
class Input;
class Camera;
class Model;
class AttackManager;
class UIManager;
class Heart :
	public ItemBase, public std::enable_shared_from_this<Heart>
{
public:
	Heart(int modelHandle, Vector3 pos);
	virtual ~Heart();
	//�o�^����
	void Entry(std::shared_ptr<ActorManager> actorManager)override;
	//�o�^����
	void Exit(std::shared_ptr<ActorManager> actorManager)override;
	//����������
	void Init()override;
	//�X�V����
	void Update(const Input& input, const std::unique_ptr<Camera>& camera, const std::shared_ptr<ActorManager> actorManager) override;
	//�d��
	void Gravity(const Vector3& gravity)override;
	//�Փ˃C�x���g�֐�
	void OnHitColl(const std::shared_ptr<Collidable>& other)override;
	//�`��
	void Draw()const override;
	//�X�V�����ɂ�錋�ʂ̊m��
	void Complete() override;
private:
	//���f��
	std::shared_ptr<Model> m_model;
	//�ŏ��̐��t���[���͓����蔻����������Ȃ��̂�
	int m_noHitFrame;
};

