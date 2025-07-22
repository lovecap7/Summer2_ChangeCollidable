#include "TitlePlayer.h"
#include <DxLib.h>
#include "../../General/Model.h"
#include "../../General/Input.h"

namespace
{
	//待機アニメ
	const char* kIdleAnim = "Anim|Idle";
	//歩く
	const char* kWalkAnim = "Anim|Walk";
}

TitlePlayer::TitlePlayer()
{
	//モデル
	m_model = std::make_unique<Model>(MV1LoadModel(L"Data/Model/Player/Title/TitlePlayer.mv1"), Vector3::Zero().ToDxLibVector());
}

TitlePlayer::~TitlePlayer()
{
}

void TitlePlayer::Init()
{
	//アニメーション
	m_model->SetAnim(kIdleAnim, true);
}

void TitlePlayer::Update()
{
	auto& Input = Input::GetInstance().GetInstance();
	if (Input.IsTriggerAny())
	{
		//アニメーションを歩くに変更
		m_model->SetAnim(kWalkAnim, false);
	}
	m_model->Update();
}

void TitlePlayer::Draw() const
{
	//モデルを描画
	m_model->Draw();
}

void TitlePlayer::End()
{
	//モデルの終了処理
	m_model->End();
}
