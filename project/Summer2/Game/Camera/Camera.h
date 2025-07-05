#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class ActorManager;
class Rigidbody;
class Camera
{
public:
	Camera();
	~Camera();
	//����������
	void Init();
	//�X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager);
	//�J�����̍��W
	Position3 GetPos() { return m_pos; }
	//�J�����̌���
	Vector3 GetDir();
private:
	//�����̍��W
	Position3 m_pos;
	//�J�����̌���
	Vector3 m_dir;
	//�J���������Ă�ʒu
	Vector3 m_viewPos;
	//�N���A���̉�]�t���[��
	int m_rotaFrame;
private:
	//��ԑJ��
	using UpdateFunc_t = void(Camera::*)(const std::weak_ptr<ActorManager> actorManager);
	UpdateFunc_t m_update;
	//�ʏ�̃J����
	void NormalUpdate(const std::weak_ptr<ActorManager> actorManager);
	//�{�X��̃J����
	void BossUpdate(const std::weak_ptr<ActorManager> actorManager);
	//�N���A���̃J����
	void GameClearUpdate(const std::weak_ptr<ActorManager> actorManager);
};

