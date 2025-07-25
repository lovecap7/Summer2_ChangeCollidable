#include "TitleUI.h"
#include "../../../General/game.h"
#include "../../../General/Input.h"
#include "../../../Shader/MyDraw.h"
#include <DxLib.h>
#include <assert.h>

namespace
{
	//タイトル
	constexpr float kTitlePosX = Game::kScreenWidth / 2;
	constexpr float kTitlePosY = 250;
	//PAB
	constexpr float kPressAnyBottunPosX = Game::kScreenWidth / 2;
	constexpr float kPressAnyBottunPosY = 600;
	//点滅全体フレーム
	constexpr int kBlinkingFrame = 60;
	//非表示
	constexpr int kShutFrame = kBlinkingFrame / 2;
	//ディゾルブの速度
	constexpr float kDissolveSpeed = 0.01f;
	//ディゾルブの最大値
	constexpr float kMaxDissolveRate = 1.5f;  //完全に消える状態
	//ディゾルブの最小値
	constexpr float kMinDissolveRate = 0.0f; //完全に出現する状態
}

TitleUI::TitleUI() :
	UIBase(),
	m_countFrame(0),
	m_titleHandle(LoadGraph(L"Data/UI/Title/Title.png")),
	m_titleBackHandle(LoadGraph(L"Data/UI/Title/TitleBack.png")),
	m_pressAnyBottunHandle(LoadGraph(L"Data/UI/Title/PressAnyBottun.png")),
	m_pressAnyBottunBackHandle(LoadGraph(L"Data/UI/Title/PressAnyBottunBack.png")),
	m_isDecide(false),
	m_isAppeared(false),
	m_dissolveRate(kMaxDissolveRate)
{
	m_patternHandle = LoadGraph(L"Data/ShaderSample/pattern6.png");
	m_pixelShaderHandle = LoadPixelShader(L"Shader/DissolvePS.pso");
	assert(m_titleHandle != -1);
	assert(m_titleBackHandle != -1);
	assert(m_pressAnyBottunHandle != -1);
	assert(m_pressAnyBottunBackHandle != -1);
	assert(m_patternHandle != -1);
	assert(m_pixelShaderHandle != -1);

	//定数バッファの確保
	m_constantBufferHandle = DxLib::CreateShaderConstantBuffer(sizeof(float) * 4);//16アライメント
	//グラボのメモリは実は直接いじれない。
	//RAM上のメモリでシミュレートしてる
	m_threshold = static_cast<float*>(DxLib::GetBufferShaderConstantBuffer(m_constantBufferHandle));
}

TitleUI::~TitleUI()
{
	DeleteGraph(m_titleHandle);
	DeleteGraph(m_titleBackHandle);
	DeleteGraph(m_pressAnyBottunHandle);
	DeleteGraph(m_pressAnyBottunBackHandle);
	DeleteGraph(m_patternHandle);
	DeleteShader(m_pixelShaderHandle);
}

void TitleUI::Update()
{
	auto& input = Input::GetInstance();
	if (input.IsTriggerAny())
	{
		if (m_isAppeared)
		{
			m_isDecide = true;
		}
		else
		{
			m_dissolveRate = kMinDissolveRate; // すぐに表示されるようにする
			m_isAppeared = true;
		}
	}
	//だんだんタイトルが出現する
	m_dissolveRate -= kDissolveSpeed;
	m_dissolveRate = MathSub::ClampFloat(m_dissolveRate, kMinDissolveRate, kMaxDissolveRate);
	if (m_dissolveRate <= kMinDissolveRate)
	{
		m_isAppeared = true;
		++m_countFrame;
		if (m_countFrame > kBlinkingFrame)
		{
			m_countFrame = 0;
		}
	}
	m_threshold[0] = m_dissolveRate;
	UpdateShaderConstantBuffer(m_constantBufferHandle);
	//最後の番号のレジスタに送られる
	SetShaderConstantBuffer(m_constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);

}

void TitleUI::Draw() const
{
	//決定したら少し光る
	if (m_isDecide)
	{
		DrawRotaGraph(kTitlePosX, kTitlePosY, 1.0, 0.0, m_titleBackHandle, true);
	}
	//シェーダで描画
	MyDrawUtils::DrawRotaGraph(
		Vector2(kTitlePosX, kTitlePosY),
		1.0f,
		m_dissolveRate,
		m_titleHandle,
		m_pixelShaderHandle,
		{ m_patternHandle }
	);
	//点滅
	if (m_countFrame <= kShutFrame && m_isAppeared)
	{
		DrawRotaGraph(kPressAnyBottunPosX, kPressAnyBottunPosY, 1.0, 0.0, m_pressAnyBottunBackHandle, true);
		DrawRotaGraph(kPressAnyBottunPosX, kPressAnyBottunPosY, 1.0, 0.0, m_pressAnyBottunHandle, true);
	}
}
