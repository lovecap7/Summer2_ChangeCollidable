#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class ActorManager;
class Rigidbody;
class CameraStateBase;
class Camera:
	public std::enable_shared_from_this<Camera>
{
public:
	Camera();
	~Camera();
	//����������
	void Init();
	//�X�V����
	void Update(const std::weak_ptr<ActorManager> actorManager);
	//�J�����̍��W
	Position3 GetPos() const { return m_pos; };
	void SetPos(Position3 pos) { m_pos = pos; };
	//�J�����̌���
	Vector3 GetDir()const;
	void SetDir(Vector3 dir);
	//�J�����̌��Ă���ʒu
	Vector3 GetViewPos()const { return m_viewPos; };
	void SetViewPos(Vector3 viewPos) { m_viewPos = viewPos; };

	//�C�x���g�G���A���ł̏����ɕK�v
	//�C�x���g�J�n
	void EventStart(float startPosX, float endPosX);
	//�C�x���g�I��
	void EventEnd();
	//�C�x���g��
	bool IsEvent()const { return m_isEvent; };
	//�X�^�[�g�ƃG���h��X���W
	Vector2 StartXAndEndX()const { return m_startXAndEndX; };
private:
	//�����̍��W
	Position3 m_pos;
	//�J�����̌���
	Vector3 m_dir;
	//�J���������Ă�ʒu
	Vector3 m_viewPos;
	//�C�x���g���t���O
	bool m_isEvent;
	//�G���A��X���W
	Vector2 m_startXAndEndX;
private:
	//��ԑJ��
	std::shared_ptr<CameraStateBase> m_state;
};

