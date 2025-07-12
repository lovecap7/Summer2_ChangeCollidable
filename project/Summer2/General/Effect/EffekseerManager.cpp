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
	//�a��
	if (name == "SlashtTrajectEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["SlashtTrajectEff"]), pos);
	}
	//�ʏ�q�b�g�G�t�F�N�g
	else if (name == "ImpactHitEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["ImpactHitEff"]), pos);
	}
	//�a���q�b�g�G�t�F�N�g
	else if (name == "SlashHitEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["SlashHitEff"]), pos);
	}
	//�`���[�W
	else if (name == "ChargeEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["ChargeEff"]), pos);
	}

	Entry(effect);
	return effect;
}

std::weak_ptr<TrackActorEffect> EffekseerManager::CreateTrackActorEffect(std::string name, std::weak_ptr<Actor> actor)
{
	std::shared_ptr<TrackActorEffect> effect;
	//�U���o�t
	if (name == "PowerUp")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["PowerUp"]), actor);
	}
	//�`���[�W�U��
	else if (name == "CATornade")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["CATornade"]), actor);
	}
	//�`���[�W���x��1
	else if (name == "ChargeLevel1Eff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["ChargeLevel1Eff"]), actor);
	}
	//�`���[�W���x��2
	else if (name == "ChargeLevel2Eff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["ChargeLevel2Eff"]), actor);
	}
	//�`���[�W���x��3
	else if (name == "ChargeLevel3Eff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["ChargeLevel3Eff"]), actor);
	}
	Entry(effect);
	return effect;
}

void EffekseerManager::LoadHandle()
{
	m_handles["PowerUp"] = { LoadEffekseerEffect("Data/Effects/PowerUp.efkefc") };
	m_handles["SlashtTrajectEff"] = { LoadEffekseerEffect("Data/Effects/SlashtTrajectEff.efkefc") };
	m_handles["CATornade"] = { LoadEffekseerEffect("Data/Effects/CATornade.efkefc") };
	m_handles["ChargeEff"] = { LoadEffekseerEffect("Data/Effects/ChargeEff.efkefc") };
	m_handles["ChargeLevel1Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel1Eff.efkefc") };
	m_handles["ChargeLevel2Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel2Eff.efkefc") };
	m_handles["ChargeLevel3Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel3Eff.efkefc") };
	m_handles["SlashHitEff"] = { LoadEffekseerEffect("Data/Effects/SlashHitEffect.efkefc") };
	m_handles["ImpactHitEff"] = { LoadEffekseerEffect("Data/Effects/ImpactHitEff.efkefc") };
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