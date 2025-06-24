#pragma once
#include "Math/Vector3.h"
//��{16:9 (4:3��OK)
//���̃T�C�Y�ȊO�͓��ʂȗ��R���Ȃ������{�_��
namespace Game//����
{
	//��ʂ̑傫����萔��`	16:9
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	constexpr int kColorBitNum = 32;

	constexpr bool kDefaultWindowMode = true;

	//�v���C���[�̃R���g���[���[�C���f�b�N�X
	enum class PlayerIndex
	{
		None,
		Player1,
		Player2,
		Player3,
		Player4,
	};
}

namespace Gravity
{
	//�d��
	const Vector3 kGravity = { 0.0f, -0.5f, 0.0f };
	//������Ԃɐ؂�ւ�闎���x�N�g���̑傫��
	constexpr float kChangeStateFall = -2.0f;
	//�����X�s�[�h�̏��(Y�����̑傫��)
	constexpr float kMaxGravityY = -10.0f;
}