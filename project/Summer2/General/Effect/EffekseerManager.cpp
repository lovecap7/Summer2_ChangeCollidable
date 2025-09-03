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
	//初期化
	m_isUpdate = true;
	m_delayFrame = 0;
}

void EffekseerManager::Update()
{
	//遅延処理
	UpdateDelay();
	//更新をしないなら
	if (!m_isUpdate || m_delayFrame > 0)
	{
		//再生ストップ
		StopEffect();
		return;
	}
	else
	{
		//再生
		StartEffect();
	}
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
	Reset();
	//終了
	Effkseer_End();
}

void EffekseerManager::Reset()
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
	//斬撃1
	if (name == "SlashtTraject1Eff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["SlashtTraject1Eff"]), pos);
	}
	//斬撃2
	if (name == "SlashtTraject2Eff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["SlashtTraject2Eff"]), pos);
	}
	//通常ヒットエフェクト
	else if (name == "ImpactHitEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["ImpactHitEff"]), pos);
	}
	//斬撃ヒットエフェクト
	else if (name == "SlashHitEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["SlashHitEff"]), pos);
	}
	//必殺ヒットエフェクト
	else if (name == "UltHitEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["UltHitEff"]), pos);
	}
	//チャージ
	else if (name == "ChargeEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["ChargeEff"]), pos);
	}
	//爆発
	else if (name == "ExplosionEff") 
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["ExplosionEff"]), pos);
	}
	//死亡エフェクト
	else if (name == "DeathEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["DeathEff"]), pos);
	}
	//ボス死亡エフェクト
	else if (name == "BossDeathEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BossDeathEff"]), pos);
	}
	//必殺レーザーエフェクト
	else if (name == "UltLaserEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["UltLaserEff"]), pos);
	}
	//必殺チャージエフェクト
	else if (name == "UltChargeSwordEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["UltChargeSwordEff"]), pos);
	}
	//足元の衝撃エフェクト
	else if (name == "LowShockWaveEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["LowShockWaveEff"]), pos);
	}
	//ボスのビームエフェクト
	else if (name == "BossBeamEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BossBeamEff"]), pos);
	}
	//ボス(King)のビームエフェクト
	else if (name == "BossKingBeamEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BossKingBeamEff"]), pos);
	}
	//ボスのビームチャージエフェクト
	else if (name == "BossBeamChargeEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BossBeamChargeEff"]), pos);
	}
	//ボス(king)のビームチャージエフェクト
	else if (name == "BossKingBeamChargeEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BossKingBeamChargeEff"]), pos);
	}
	//衝撃波1エフェクト
	else if (name == "WaveAttackEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["WaveAttackEff"]), pos);
	}
	//衝撃波2エフェクト
	else if (name == "MagicWaveEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["MagicWaveEff"]), pos);
	}
	//爆発魔法陣
	else if (name == "BlastMagicEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BlastMagicEff"]), pos);
	}
	//魔法陣
	else if (name == "MagicCircleEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["MagicCircleEff"]), pos);
	}
	//大きな魔法陣
	else if (name == "BigMagicCircleEff")
	{
		effect = std::make_shared<MyEffect>(PlayEffekseer3DEffect(m_handles["BigMagicCircleEff"]), pos);
	}
	
	Entry(effect);
	return effect;
}

std::weak_ptr<TrackActorEffect> EffekseerManager::CreateTrackActorEffect(std::string name, std::weak_ptr<Actor> actor)
{
	std::shared_ptr<TrackActorEffect> effect;
	//獲得攻撃バフ
	if (name == "GetAttackUpEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["GetAttackUpEff"]), actor);
	}
	//攻撃バフ
	if (name == "AttackUpEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["AttackUpEff"]), actor);
	}
	//獲得回復
	else if (name == "GetHealEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["GetHealEff"]), actor);
	}
	//獲得防御バフ
	else if (name == "GetDefenseUpEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["GetDefenseUpEff"]), actor);
	}
	//防御バフ
	else if (name == "DefenseUpEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["DefenseUpEff"]), actor);
	}
	//必殺ゲージアップ
	else if (name == "GetUltGageUpEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["GetUltGageUpEff"]), actor);
	}
	//チャージ攻撃
	else if (name == "CATornade")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["CATornade"]), actor);
	}
	//チャージレベル1
	else if (name == "ChargeLevel1Eff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["ChargeLevel1Eff"]), actor);
	}
	//チャージレベル2
	else if (name == "ChargeLevel2Eff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["ChargeLevel2Eff"]), actor);
	}
	//チャージレベル3
	else if (name == "ChargeLevel3Eff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["ChargeLevel3Eff"]), actor);
	}
	//必殺ゲージMAXエフェクト
	else if (name == "UltGageMaxEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["UltGageMaxEff"]), actor);
	}
	//弾のエフェクト
	else if (name == "BulletEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["BulletEff"]), actor);
	}
	//ブレスのエフェクト
	else if (name == "BreathEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["BreathEff"]), actor);
	}
	//ローリングエフェクト
	else if (name == "RollingEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["RollingEff"]), actor);
	}
	//フィールドエフェクト
	else if (name == "FieldEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["FieldEff"]), actor);
	}
	//怒りエフェクト
	else if (name == "BossAngryEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["BossAngryEff"]), actor);
	}
	//ボス3のオーラエフェクト
	else if (name == "BossKingStartEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["BossKingStartEff"]), actor);
	}
	//ボス3の変身オーラエフェクト
	else if (name == "BossKingTransEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["BossKingTransEff"]), actor);
	}
	//プレイヤー復活エフェクト
	else if (name == "RevivalPlayerEff")
	{
		effect = std::make_shared<TrackActorEffect>(PlayEffekseer3DEffect(m_handles["RevivalPlayerEff"]), actor);
	}
	
	Entry(effect);
	return effect;
}

void EffekseerManager::StopEffect()
{
	//止める
	m_isUpdate = false;
}

void EffekseerManager::StartEffect()
{
	//再生
	m_isUpdate = true;
}

void EffekseerManager::DelayUpdate(int frame)
{
	m_delayFrame = frame;
	m_isUpdate = false;
}

void EffekseerManager::LoadHandle()
{
	m_handles["GetAttackUpEff"] = { LoadEffekseerEffect("Data/Effects/GetAttackUpEff.efkefc") };
	m_handles["AttackUpEff"] = { LoadEffekseerEffect("Data/Effects/AttackUpEff.efkefc") };
	m_handles["GetHealEff"] = { LoadEffekseerEffect("Data/Effects/GetHealEff.efkefc") };
	m_handles["GetDefenseUpEff"] = { LoadEffekseerEffect("Data/Effects/GetDefenseUpEff.efkefc") };
	m_handles["DefenseUpEff"] = { LoadEffekseerEffect("Data/Effects/DefenseUpEff.efkefc") };
	m_handles["GetUltGageUpEff"] = { LoadEffekseerEffect("Data/Effects/GetUltGageUpEff.efkefc") };
	m_handles["SlashtTraject1Eff"] = { LoadEffekseerEffect("Data/Effects/SlashtTraject1Eff.efkefc") };
	m_handles["SlashtTraject2Eff"] = { LoadEffekseerEffect("Data/Effects/SlashtTraject2Eff.efkefc") };
	m_handles["CATornade"] = { LoadEffekseerEffect("Data/Effects/CATornade.efkefc") };
	m_handles["ChargeEff"] = { LoadEffekseerEffect("Data/Effects/ChargeEff.efkefc") };
	m_handles["ChargeLevel1Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel1Eff.efkefc") };
	m_handles["ChargeLevel2Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel2Eff.efkefc") };
	m_handles["ChargeLevel3Eff"] = { LoadEffekseerEffect("Data/Effects/ChargeLevel3Eff.efkefc") };
	m_handles["SlashHitEff"] = { LoadEffekseerEffect("Data/Effects/SlashHitEffect.efkefc") };
	m_handles["ImpactHitEff"] = { LoadEffekseerEffect("Data/Effects/ImpactHitEff.efkefc") };
	m_handles["ExplosionEff"] = { LoadEffekseerEffect("Data/Effects/ExplosionEff.efkefc") };
	m_handles["UltChargeSwordEff"] = { LoadEffekseerEffect("Data/Effects/UltChargeSwordEff.efkefc") };
	m_handles["UltLaserEff"] = { LoadEffekseerEffect("Data/Effects/UltLaserEff.efkefc") };
	m_handles["UltGageMaxEff"] = { LoadEffekseerEffect("Data/Effects/UltGageMaxEff.efkefc") };
	m_handles["UltHitEff"] = { LoadEffekseerEffect("Data/Effects/UltHitEff.efkefc") };
	m_handles["BulletEff"] = { LoadEffekseerEffect("Data/Effects/BulletEff.efkefc") };
	m_handles["BreathEff"] = { LoadEffekseerEffect("Data/Effects/BreathEff.efkefc") };
	m_handles["DeathEff"] = { LoadEffekseerEffect("Data/Effects/DeathEff.efkefc") };
	m_handles["BossDeathEff"] = { LoadEffekseerEffect("Data/Effects/BossDeathEff.efkefc") };
	m_handles["RollingEff"] = { LoadEffekseerEffect("Data/Effects/RollingEff.efkefc") };
	m_handles["LowShockWaveEff"] = { LoadEffekseerEffect("Data/Effects/LowShockWaveEff.efkefc") };
	m_handles["FieldEff"] = { LoadEffekseerEffect("Data/Effects/FieldEffect.efkefc") };
	m_handles["BossBeamEff"] = { LoadEffekseerEffect("Data/Effects/BossBeamEff.efkefc") };
	m_handles["BossBeamChargeEff"] = { LoadEffekseerEffect("Data/Effects/BossBeamChargeEff.efkefc") };
	m_handles["BossKingBeamEff"] = { LoadEffekseerEffect("Data/Effects/BossKingBeamEff.efkefc") };
	m_handles["BossKingBeamChargeEff"] = { LoadEffekseerEffect("Data/Effects/BossKingBeamChargeEff.efkefc") };
	m_handles["BossAngryEff"] = { LoadEffekseerEffect("Data/Effects/BossAngryEff.efkefc") };
	m_handles["WaveAttackEff"] = { LoadEffekseerEffect("Data/Effects/WaveAttackEff.efkefc") };
	m_handles["MagicWaveEff"] = { LoadEffekseerEffect("Data/Effects/MagicWaveEff.efkefc") };
	m_handles["BossKingStartEff"] = { LoadEffekseerEffect("Data/Effects/BossKingStartEff.efkefc") };
	m_handles["BossKingTransEff"] = { LoadEffekseerEffect("Data/Effects/BossKingTransEff.efkefc") };
	m_handles["BlastMagicEff"] = { LoadEffekseerEffect("Data/Effects/BlastMagicEff.efkefc") };
	m_handles["MagicCircleEff"] = { LoadEffekseerEffect("Data/Effects/MagicCircleEff.efkefc") };
	m_handles["BigMagicCircleEff"] = { LoadEffekseerEffect("Data/Effects/BigMagicCircleEff.efkefc") };
	m_handles["RevivalPlayerEff"] = { LoadEffekseerEffect("Data/Effects/RevivalPlayerEff.efkefc") };
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
void EffekseerManager::UpdateDelay()
{
	if (m_delayFrame > 0)
	{
		--m_delayFrame;
		if (m_delayFrame <= 0)
		{
			m_isUpdate = true;
		}
	}
}