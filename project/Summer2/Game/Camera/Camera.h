#pragma once
#include <memory>
#include "../../General/Math/MyMath.h"
class Actor;
class Rigidbody;
class Camera
{
public:
	Camera(Position3 firstPos, std::shared_ptr<Actor> player);
	~Camera();
	void Update();
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
	//�v���C���[
	std::shared_ptr<Actor> m_player;
	//�J�����̍ŏ��̂y���W(�J������Z�����̈ړ�����Ɏg��)
	float m_cameraFirstPosZ;
};

