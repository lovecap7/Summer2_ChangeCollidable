#pragma once
#include "../Math/Vector3.h"
#include <memory>

class Actor;
class Collidable;
class SearchTrigger
{
public:
	SearchTrigger(float radius, float viewAngle,std::shared_ptr<Actor> owner);
	SearchTrigger(float radius ,std::shared_ptr<Actor> owner);
	~SearchTrigger();
	//������
	void Init();
	//�g���K�[
	std::shared_ptr<Collidable> GetCollidable() { return m_serchTrigger; };
	//����p
	float GetViewAngle() { return m_viewAngle; };
	//�O�x�N�g��
	Vector3 GetViewForward() { return m_forward; };
	void SetViewForward(Vector3 forward) { m_forward = forward; };
	//�ʒu�X�V
	void SetPos(Vector3 pos);
	//�^�[�Q�b�g��ۑ�
	void OnHitTarget(Vector3 target);
	Vector3 GetTargetPos() { return m_target; };
	bool IsTargetHit() { return m_isHit; };
private:
	//�O
	Vector3 m_forward;
	//����p
	float m_viewAngle;
	//�g���K�[
	std::shared_ptr<Collidable> m_serchTrigger;
	//���G�ɐ���������
	bool m_isHit;
	//�^�[�Q�b�g�̍��W
	Vector3 m_target;
};

