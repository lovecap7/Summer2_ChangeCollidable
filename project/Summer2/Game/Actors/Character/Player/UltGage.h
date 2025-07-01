#pragma once
class UltGage
{
public:
	UltGage(int max);
	~UltGage();
	//���̃Q�[�W�ʂ�Ԃ�
	int GetUltGageValue() { return m_ultGageValue; };
	//�Q�[�W�����Z
	void AddUltGage(int add);
	//�Q�[�W���Z�b�g
	void ResetUltGage() { m_ultGageValue = 0; };
	//�K�E�Z�Q�[�W���}�b�N�X�Ȃ�true
	bool IsMaxUlt() { return m_ultGageValue >= m_maxUltGageValue; };
	//�Q�[�W�̍ő�l
	int GetMaxUltGageValue() { return m_maxUltGageValue; };
	//���Z�����\��̒l���Z�b�g
	void SetPendingUltGage(int pendingUltGage) { m_pendingUltGauge = pendingUltGage; };
	//�\��l�̔��f
	void AddPedingUltGage() { AddUltGage(m_pendingUltGauge); };
private:
	int m_ultGageValue;
	int m_maxUltGageValue;
	//���Z�����\��̒l(�U���������������ɉ��Z�����)
	int m_pendingUltGauge;
};

