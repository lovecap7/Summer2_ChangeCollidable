#include "EffekseerManager.h"
#include <EffekseerForDXLib.h>

namespace
{
	constexpr int kMaxParticleNum = 8000;
}

void EffekseerManager::Init()
{
	//パーティクル数を設定
	Effekseer_Init(kMaxParticleNum);
	//歪みエフェクトの初期化
	Effekseer_InitDistortion();
	//フルスク切り替え時におかしくならないように設定
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

void EffekseerManager::Update()
{
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
	Effkseer_End();
}
