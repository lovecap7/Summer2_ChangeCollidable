#pragma once
class Animator
{
private:
	struct Anim
	{
		int m_attachAnim = -1;//�Đ����̃A�j���[�V������
		int m_attachAnimIndex = -1;//�A�j���[�V�����̃A�^�b�`���ꂽ�ԍ�
		float m_animTimer = 0.0f;//�A�j���[�V�����̃^�C�}�[
		float m_animStopTime = 0.0f;//�A�j���[�V�����̏I���^�C�}�[
		bool m_isLoopAnim = false;//���[�v����A�j���[�V�������ǂ���
		bool m_isFinishAnim = false;//�A�j���[�V�������I�������true
		float m_animSpeed = 0.5f;//�Đ����x
		//���t���[���ԃ��[�v�Đ�
		bool m_isFixedLoop = false;
		float m_fixedLoopFrame = 0.0f;
	};
	Anim m_animNow;//���̃A�j���[�V����
	Anim m_animNext;//���̃A�j���[�V����
	//�u�����h�Ɏg��
	float m_blendRate;
	//�A�j���[�V������i�߂�
	void UpdateAnim(const int& modelHandle, Anim& anim);
	//�u�����h���̍X�V
	void UpdateBlend(const int& modelHandle);
public:
	//�R���X�g���N�^
	Animator();
	//�f�X�g���N�^
	~Animator();

	/// <summary>
	/// �A�j���[�V�����̃A�^�b�`
	/// </summary>
	/// <param name="modelHandle">���f���̃n���h��</param>
	/// <param name="anim">animation enum</param>
	/// <param name="isLoop">���[�v���邩</param>
	void SetAnim(const int& modelHandle, const int& anim, const bool& isLoop);

	//�A�j���[�V�����̃A�^�b�`�ƍĐ����x�ύX
	void SetAnim(const int& modelHandle, const int& anim, const bool& isLoop,const float& animSpeed);

	/// <summary>
	/// �ŏ�����Đ�
	/// </summary>
	void Replay();

	/// <summary>
	/// �w�肳�ꂽ�A�j���[�V�������Đ����Ȃ�폜
	/// </summary>
	/// <param name="modelHandle">���f���̃n���h��</param>
	/// <param name="anim">animation enum</param>
	void RemoveAnim(const int& modelHandle, Anim& anim);

	/// <summary>
	/// �A�j���[�V������S�č폜
	/// </summary>
	/// <param name="modelHandle"></param>
	void AllRemoveAnim(const int& modelHandle);

	/// <summary>
	/// �A�j���[�V�����̍Đ�
	/// </summary>
	/// <param name="modelHandle"></param>
	void PlayAnim(const int& modelHandle);

	/// <summary>
	/// �A�j���[�V�������I��������ǂ���
	/// </summary>
	/// <returns></returns>
	bool IsFinishAnim();

	//�A�j���[�V�������I���t���[��(�S�̃t���[�� / �Đ����x)
	float GetTotalAnimFrame() { return  m_animNext.m_animStopTime / m_animNext.m_animSpeed; };
	//�A�j���t���[��(�t���[�� / �Đ����x)
	float GetAnimFrame() { return  m_animNext.m_animTimer / m_animNext.m_animSpeed; };
	//�A�j���[�V�����̑S�̃t���[�����Z�b�g����
	void SetTotalAnimFrame(float totalFrame) { m_animNext.m_animStopTime = totalFrame; };
	//���[�v�t���[�����Z�b�g����
	void SetFixedLoop(float loopFrame);
	//���[�v�Đ����I��������
	bool IsFinishFixedLoop();
	//�A�j���[�V�����̑��x��ύX
	void SetAnimSpeed(const float& animSpeed);
};
