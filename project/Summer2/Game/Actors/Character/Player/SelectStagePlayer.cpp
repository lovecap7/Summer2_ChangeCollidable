#include "SelectStagePlayer.h"
#include <DxLib.h>
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"

namespace
{
	//ダンス
	const char* kDance1Anim = "Select|Dance1";
	const char* kDance2Anim = "Select|Dance2";
	const char* kDance3Anim = "Select|Dance3";
	//走る
	const char* kRunAnim = "Select|Run";
	//決定
	const char* kDecideAnim = "Select|Decide";
}

SelectStagePlayer::SelectStagePlayer():
	CharacterBase(Shape::None)
{
	//モデル
	m_model = std::make_shared<Model>(MV1LoadModel(L"Data/Model/Player/SelectStage/SelectStagePlayer.mv1"), Vector3::Zero().ToDxLibVector());
}

SelectStagePlayer::~SelectStagePlayer()
{
}

void SelectStagePlayer::Init()
{
	//アニメーション
	m_model->SetAnim(kDance3Anim, true);
}

void SelectStagePlayer::Update()
{
	
	m_model->Update();
}

void SelectStagePlayer::Draw() const
{
	//モデルを描画
	m_model->Draw();
}

void SelectStagePlayer::End()
{
	//モデルの終了処理
	m_model->End();
}
