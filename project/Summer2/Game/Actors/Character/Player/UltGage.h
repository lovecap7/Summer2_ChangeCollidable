#pragma once
class UltGage
{
public:
	UltGage(int max);
	~UltGage();
	//���̃Q�[�W�ʂ�Ԃ�
	int GetUltGage() { return m_ultGage; };
	//�Q�[�W�����Z
	void AddUltGage(int add);
	//�Q�[�W���Z�b�g
	void ResetUltGage() { m_ultGage = 0; };
	//�K�E�Z�Q�[�W���}�b�N�X�Ȃ�true
	bool IsMaxUlt() { return m_ultGage >= m_maxUltGage; };
	//�Q�[�W�̍ő�l
	int GetMaxUltGage() { return m_maxUltGage; };
	//���Z�����\��̒l���Z�b�g
	void SetPendingUltGage(int pendingUltGage) { m_pendingUltGauge = pendingUltGage; };
	//�\��l�̔��f
	void AddPedingUltGage() { AddUltGage(m_pendingUltGauge); };
private:
	int m_ultGage;
	int m_maxUltGage;
	//���Z�����\��̒l(�U���������������ɉ��Z�����)
	int m_pendingUltGauge;
};

