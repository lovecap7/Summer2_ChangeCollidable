#include "EffekseerManager.h"
#include "MyEffect.h"
#include <EffekseerForDXLib.h>

namespace
{
	constexpr int kMaxParticleNum = 8000;
}

void EffekseerManager::Entry(std::shared_ptr<MyEffect> eff)
{
	//���łɓo�^����Ă���Ȃ炵�Ȃ�
	auto it = std::find(m_effects.begin(), m_effects.end(), eff);
	if (it != m_effects.end())return;
	//�o�^
	m_effects.emplace_back(eff);
}

void EffekseerManager::Exit(std::shared_ptr<MyEffect> eff)
{
	//�o�^����Ă��Ȃ��Ȃ炵�Ȃ�
	auto it = std::find(m_effects.begin(), m_effects.end(), eff);
	if (it == m_effects.end())return;
	//����
	m_effects.remove(eff);
}
void EffekseerManager::Init()
{
	//�p�[�e�B�N������ݒ�
	Effekseer_Init(kMaxParticleNum);
	//�c�݃G�t�F�N�g�̏�����
	Effekseer_InitDistortion();
	//�t���X�N�؂�ւ����ɂ��������Ȃ�Ȃ��悤�ɐݒ�
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

void EffekseerManager::Update()
{
	//�S�ẴG�t�F�N�g�̍X�V
	UpdateEffekseer3D();
}

void EffekseerManager::Draw() const
{
	//Effekseer��3D�\���̐ݒ��DX���C�u������3D�\���̐ݒ�ɓ���������
	Effekseer_Sync3DSetting();
	//�S�ẴG�t�F�N�g�̕`��
	DrawEffekseer3D();
}

void EffekseerManager::End()
{
	Effkseer_End();
}
