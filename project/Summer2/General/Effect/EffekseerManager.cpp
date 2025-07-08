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
	//すでに登録されているならしない
	auto it = std::find(m_effects.begin(), m_effects.end(), eff);
	if (it != m_effects.end())return;
	//登録
	m_effects.emplace_back(eff);
}

void EffekseerManager::Exit(std::shared_ptr<MyEffect> eff)
{
	//登録されていないならしない
	auto it = std::find(m_effects.begin(), m_effects.end(), eff);
	if (it == m_effects.end())return;
	//解除
	m_effects.remove(eff);
}
void EffekseerManager::Init()
{
	//パーティクル数を設定
	Effekseer_Init(kMaxParticleNum);
	//歪みエフェクトの初期化
	Effekseer_InitDistortion();
	//フルスク切り替え時におかしくならないように設定
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	//ハンドルロード
	LoadHandle();
}

void EffekseerManager::Update()
{
	//更新
	for (auto& eff : m_effects)
	{
		eff->Update();
	}
	//削除予定のものを削除
	CheckDeleteEffect();
	//全てのエフェクトの更新
	UpdateEffekseer3D();
}

void EffekseerManager::Draw() const
{
	//Effekseerに3D表示の設定をDXライブラリの3D表示の設定に同期させる
	Effekseer_Sync3DSetting();
	//全てのエフェクトの描画
	DrawEffekseer3D();
}

void EffekseerManager::End()
{
	//ハンドル削除
	AllDeleteHandle();
	//エフェクト削除
	AllDeleteEffect();
	//終了
	Effkseer_End();
}

void EffekseerManager::AllDeleteEffect()
{
	//削除
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
	//攻撃バフ
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
	//ロードに成功したかチェック
	for (const auto& [key, value] : m_handles) {
		assert(value >= 0);
	}
}

void EffekseerManager::AllDeleteHandle()
{
	//ハンドルをすべて削除
	for (const auto& [key, value] : m_handles) {
		DeleteEffekseerEffect(value);
	}
	m_handles.clear();
}


//エフェクトの消滅フラグをチェックして削除
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
				//終了処理
				eff->End();
				//削除候補
				deleteEffect.emplace_back(eff);
			}
		}
		//削除
		for (auto& eff : deleteEffect)
		{
			Exit(eff);
		}
		deleteEffect.clear();
		if (!isOneMore)break;
	}
}