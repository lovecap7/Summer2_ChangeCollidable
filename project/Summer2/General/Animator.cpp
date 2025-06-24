#include "Animator.h"
#include <DxLib.h>
namespace
{
	constexpr float kMaxBlend = 1.0f;//�u�����h���̍ő�
	constexpr float kBlendRate = 0.1f;//�u�����h��
	constexpr float kDefAnimSpeed = 0.5f;//�f�t�H���g�̍Đ����x
}

Animator::Animator():
	m_blendRate(kMaxBlend)
{
}

Animator::~Animator()
{
}

void Animator::SetAnim(const int& modelHandle, const int& anim, const bool& isLoop)
{
	//���C�����T�u�̃A�j���[�V�����Ɠ����Ȃ�ݒ肵�Ȃ�
	if (anim == m_animNext.m_attachAnim)return;

	if (m_animNow.m_attachAnim != -1)
	{
		//�Â��A�j���[�V�����͏���
		RemoveAnim(modelHandle, m_animNow);
	}
	//�V�����A�j���[�V�������Â��A�j���[�V�����ɂ���
	m_animNow = m_animNext;

	//�V�������[�V�����ɍX�V
	m_animNext.m_attachAnimIndex = MV1AttachAnim(modelHandle, anim, -1, false);
	m_animNext.m_attachAnim = anim;//���̃A�j���[�V�����̔ԍ�
	m_animNext.m_animStopTime = MV1GetAttachAnimTotalTime(modelHandle, m_animNext.m_attachAnimIndex);//�A�j���[�V�����̏I������
	m_animNext.m_animTimer = 0.0f;//�^�C�}�[������
	m_animNext.m_isLoopAnim = isLoop;//���[�v���邩
	m_animNext.m_isFinishAnim = false;//�Đ��I��
	m_animNext.m_animSpeed = kDefAnimSpeed;//�Đ����x
	//�w�胋�[�v
	m_animNext.m_isFixedLoop = false;
	m_animNext.m_fixedLoopFrame = 0.0f;

	//�u�����h�����Z�b�g
	m_blendRate = 0.0f;

	//�u�����h
	MV1SetAttachAnimBlendRate(modelHandle, m_animNow.m_attachAnim, kMaxBlend - m_blendRate);
	MV1SetAttachAnimBlendRate(modelHandle, m_animNext.m_attachAnim, m_blendRate);
}

void Animator::SetAnim(const int& modelHandle, const int& anim, const bool& isLoop, const float& animSpeed)
{
	//���C�����T�u�̃A�j���[�V�����Ɠ����Ȃ�ݒ肵�Ȃ�
	if (anim == m_animNext.m_attachAnim)return;

	if (m_animNow.m_attachAnim != -1)
	{
		//�Â��A�j���[�V�����͏���
		RemoveAnim(modelHandle, m_animNow);
	}
	//�V�����A�j���[�V�������Â��A�j���[�V�����ɂ���
	m_animNow = m_animNext;

	//�V�������[�V�����ɍX�V
	m_animNext.m_attachAnimIndex = MV1AttachAnim(modelHandle, anim, -1, false);
	m_animNext.m_attachAnim = anim;//���̃A�j���[�V�����̔ԍ�
	m_animNext.m_animStopTime = MV1GetAttachAnimTotalTime(modelHandle, m_animNext.m_attachAnimIndex);//�A�j���[�V�����̏I������
	m_animNext.m_animTimer = 0.0f;//�^�C�}�[������
	m_animNext.m_isLoopAnim = isLoop;//���[�v���邩
	m_animNext.m_isFinishAnim = false;//�Đ��I��
	m_animNext.m_animSpeed = animSpeed;//�Đ����x
	//�w�胋�[�v
	m_animNext.m_isFixedLoop = false;
	m_animNext.m_fixedLoopFrame = 0.0f;
	//�u�����h�����Z�b�g
	m_blendRate = 0.0f;

	//�u�����h
	MV1SetAttachAnimBlendRate(modelHandle, m_animNow.m_attachAnim, kMaxBlend - m_blendRate);
	MV1SetAttachAnimBlendRate(modelHandle, m_animNext.m_attachAnim, m_blendRate);
}

void Animator::Replay()
{
	//���̃��[�V�������ŏ�����Đ�
	m_animNow.m_animTimer = 0.0f;
}

void Animator::RemoveAnim(const int& modelHandle, Anim& anim)
{
	//�������������A�^�b�`����Ă��Ȃ��Ȃ瑁�����^�[��
	if (anim.m_attachAnimIndex == -1)return;
	
	//���̃��[�V����������
	MV1DetachAnim(modelHandle, anim.m_attachAnimIndex);
	anim.m_attachAnim = -1;
	anim.m_attachAnimIndex = -1;
	anim.m_animStopTime = 0.0f;
	anim.m_animTimer = 0.0f;
	anim.m_isLoopAnim = false;
	anim.m_isFinishAnim = false;
	anim.m_isFixedLoop = false;
	anim.m_fixedLoopFrame = 0.0f;
}

void Animator::AllRemoveAnim(const int& modelHandle)
{
	RemoveAnim(modelHandle, m_animNow);
	RemoveAnim(modelHandle, m_animNext);
}

void Animator::PlayAnim(const int& modelHandle)
{
	//�X�V
	UpdateBlend(modelHandle);
	UpdateAnim(modelHandle, m_animNow);
	UpdateAnim(modelHandle, m_animNext);
}

bool Animator::IsFinishAnim()
{
	return m_animNext.m_isFinishAnim;
}

void Animator::SetFixedLoop(float loopFrame)
{
	//�w�胋�[�v
	m_animNext.m_isFixedLoop = true;
	m_animNext.m_fixedLoopFrame = loopFrame;
}

bool Animator::IsFinishFixedLoop()
{
	//�w�胋�[�v�Đ����I��������true
	return m_animNext.m_isFixedLoop && m_animNext.m_fixedLoopFrame <= 0.0f;
}

void Animator::SetAnimSpeed(const float& animSpeed)
{
	m_animNext.m_animSpeed = animSpeed;
}

void Animator::UpdateAnim(const int& modelHandle,Anim& anim)
{
	//�����A�^�b�`����ĂȂ��Ȃ�Đ����Ȃ�
	if (anim.m_attachAnimIndex == -1)return;

	//�A�j���[�V�������I����Ă��Ȃ���������Ȃ��̂�false
	anim.m_isFinishAnim = false;
	//�A�j���[�V�����̏I���܂ōĐ�(�w�胋�[�v�Đ��̏ꍇ�w�胋�[�v�t���[�����I��������)
	if ((anim.m_animStopTime <= anim.m_animTimer) || (anim.m_isFixedLoop && anim.m_fixedLoopFrame <= 0.0f))
	{
		//���[�v����(�w�胋�[�v�Đ��̏ꍇ�w�胋�[�v�t���[�����I�����Ă��Ȃ��Ȃ�)
		if (anim.m_isLoopAnim || (anim.m_isFixedLoop && anim.m_fixedLoopFrame > 0.0f))
		{
			//�ŏ�����
			anim.m_animTimer = 0.0f;
			//�I��肪�Ȃ��̂ŏI��������Ƃɂ��Ă���
			anim.m_isFinishAnim = true;
		}
		else
		{
			//�I�������true
			anim.m_isFinishAnim = true;
		}
	}
	//�A�j���[�V������i�߂�
	MV1SetAttachAnimTime(modelHandle, anim.m_attachAnimIndex, anim.m_animTimer);
	anim.m_animTimer += anim.m_animSpeed;
	//�w�胋�[�v�Đ��Ȃ�
	if (anim.m_isFixedLoop && anim.m_fixedLoopFrame > 0.0f)
	{
		--anim.m_fixedLoopFrame;//���[�v�t���[�������炵�Ă���
	}
}

void Animator::UpdateBlend(const int& modelHandle)
{
	//�A�j���[�V�������Ȃ��Ȃ�u�����h���Ȃ�
	if (m_animNow.m_attachAnim == -1)return;
	if (m_animNext.m_attachAnim == -1)return;
	//�u�����h
	MV1SetAttachAnimBlendRate(modelHandle, m_animNow.m_attachAnimIndex, kMaxBlend - m_blendRate);
	MV1SetAttachAnimBlendRate(modelHandle, m_animNext.m_attachAnimIndex, m_blendRate);
	//���񂾂�ω����Ă���
	m_blendRate += kBlendRate;
	//�u�����h�����ő�Ȃ�
	if (m_blendRate >= kMaxBlend)
	{
		m_blendRate = kMaxBlend;
	}
}
