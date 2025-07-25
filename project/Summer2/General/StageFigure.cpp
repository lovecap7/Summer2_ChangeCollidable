#include "StageFigure.h"
#include "../General/Model.h"
StageFigure::StageFigure()
{
}

StageFigure::~StageFigure()
{
}

void StageFigure::Init(int handle, Vector3 pos)
{
	//モデルの初期化
	m_model = std::make_shared<Model>(handle, pos.ToDxLibVector());
}

void StageFigure::Update()
{
	//アニメーションの更新
	m_model->Update();
}

void StageFigure::Draw() const
{
	//モデルを描画
	m_model->Draw();
}


void StageFigure::End()
{
	//モデルの終了処理
	m_model->End();
}
