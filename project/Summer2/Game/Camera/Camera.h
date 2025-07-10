#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class ActorManager;
class Rigidbody;
class EventArea;
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
	//�C�x���g�G���A
	std::weak_ptr<EventArea> GetEventArea()const { return m_eventArea; };
	void SetEventArea(std::weak_ptr<EventArea> event) { m_eventArea = event; };
private:
	//�����̍��W
	Position3 m_pos;
	//�J�����̌���
	Vector3 m_dir;
	//�J���������Ă�ʒu
	Vector3 m_viewPos;
	//�C�x���g�G���A
	std::weak_ptr<EventArea> m_eventArea;
private:
	//��ԑJ��
	std::shared_ptr<CameraStateBase> m_state;
};

