#include "EffekseerManager.h"
#include "MyEffect.h"
#include "TrackActorEffect.h"
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
	//�n���h�����[�h
	LoadHandle();
}

void EffekseerManager::Update()
{
	//�X�V
	for (auto& eff : m_effects)
	{
		eff->Update();
	}
	//�폜�\��̂��̂��폜
	CheckDeleteEffect();
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
	//�n���h���폜
	AllDeleteHandle();
	//�G�t�F�N�g�폜
	AllDeleteEffect();
	//�I��
	Effkseer_End();
}

void EffekseerManager::AllDeleteEffect()
{
	//�폜
	for (auto& eff : m_effects)
	{
		eff->End();
	}
	m_effects.clear();
}

std::weak_ptr<MyEffect> EffekseerManager::CreateEffect(std::string name, Vector3 pos)
{
	std::shared_ptr<MyEffect> effect;
	if (name == "NAEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["NAEff"]), pos);
	}
	else if (name == "CAKickEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["CAKickEff"]), pos);
	}
	Entry(effect);
	return effect;
}

std::weak_ptr<MyEffect> EffekseerManager::CreateTrackActorEffect(std::string name, std::weak_ptr<Actor> actor)
{
	std::shared_ptr<MyEffect> effect;
	//�U���o�t
	if (name == "PowerUp")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["PowerUp"]), actor);
	}
	Entry(effect);
	return effect;
}

void EffekseerManager::LoadHandle()
{
	m_handles["PowerUp"] = { LoadEffekseerEffect("Data/Effects/PowerUp.efkefc") };
	m_handles["NAEff"] = { LoadEffekseerEffect("Data/Effects/NAEff.efkefc") };
	m_handles["CAKickEff"] = { LoadEffekseerEffect("Data/Effects/CAKickEff.efkefc") };
	//���[�h�ɐ����������`�F�b�N
	for (const auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

void EffekseerManager::AllDeleteHandle()
{
	//�n���h�������ׂč폜
	for (const auto& [key, value] : m_handles) {
		DeleteEffekseerEffect(value);
	}
	m_handles.clear();
}


//�G�t�F�N�g�̏��Ńt���O���`�F�b�N���č폜
void EffekseerManager::CheckDeleteEffect()
{
	std::list<std::shared_ptr<MyEffect>> deleteEffect;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		for (auto& eff : m_effects)
		{
			if (eff->IsDelete())
			{
				isOneMore = true;
				//�I������
				eff->End();
				//�폜���
				deleteEffect.emplace_back(eff);
			}
		}
		//�폜
		for (auto& eff : deleteEffect)
		{
			Exit(eff);
		}
		deleteEffect.clear();
		if (!isOneMore)break;
	}
}