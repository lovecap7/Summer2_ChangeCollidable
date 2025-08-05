#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"
#include  "../General/Collision/Physics.h"
#include  "../General/Effect/EffekseerManager.h"
#include  "../Game/UI/UIManager.h"
#include  "../General/Fader.h"
#include  "../General/SoundManager.h"

Application& Application::GetInstance()
{
    //この時点でメモリが確保されてアプリ終了まで残る
    static Application app;
    return app;
}

bool Application::Init()
{
	//ゲームタイトル
	SetWindowText(L"Spinning Knight");
	//ゲームアイコン
	//SetWindowIconID(IDI_ICON1);

	 //フルスクリーンでなく、ウィンドウモードで開くようにする
	//こういった関数はウィンドウが開く前に(Dxlib.Init()の前)に処理しておく必要がある
	ChangeWindowMode(Game::kDefaultWindowMode);

	// DirectX11を使用するようにする。(DirectX9も可、一部機能不可)
	// Effekseerを使用するには必ず設定する。
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	//画面サイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return false;			// エラーが起きたら直ちに終了
	}

	//描画先を裏画面にする
	SetDrawScreen(DX_SCREEN_BACK);

	//3D描画のための準備
	//Zバッファを使用する
	SetUseZBuffer3D(true);
	//Zバッファに書き込みを行う
	SetWriteZBuffer3D(true);
	//バックカリングを行う(ポリゴンの裏面を描画しないようにする)
	SetUseBackCulling(true);

	//Physicsの初期化
	Physics::GetInstance().Init();
	//エフェクトの初期化
	EffekseerManager::GetInstance().Init();

    return true;
}

void Application::Run()
{
	//アプリケーション以外はここで宣言と初期化
	SceneController* sceneController = new SceneController();
	//コントローラー
	auto& input = Input::GetInstance();
	input.Init();
	//Physics(衝突処理)
	auto& physics = Physics::GetInstance();
	//エフェクト
	auto& effect = EffekseerManager::GetInstance();
	//UIマネージャー
	auto& uiManager = UIManager::GetInstance();
	uiManager.Init();
	//フェード
	auto& fader = Fader::GetInstance();
	fader.Init();
	//サウンド
	auto& soundManager = SoundManager::GetInstance();
	soundManager.Init();

	//ゲームループ
	while (ProcessMessage() != -1) // Windowsが行う処理を待つ
	{
		//今回のループが始まった時間を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		//画面全体をクリア
		ClearDrawScreen();

		//ここにゲームの処理を書く
		
		//更新
		input.Update();
		sceneController->Update();
		physics.Update();
		effect.Update();
		uiManager.Update();
		fader.Update();
		soundManager.Update();
		//描画
		uiManager.BackDraw();
		sceneController->Draw();
		effect.Draw();
		uiManager.FrontDraw();
		fader.Draw();
#if _DEBUG
		DrawFormatString(0, 500, 0xff0000, L"FPS : %.2f", GetFPS());
#endif

		//画面の切り替わりを待つ必要がある
		ScreenFlip();//1/60秒経過するまで待つ

		//FPSを60に固定
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}

		//ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			sceneController = nullptr;
			break;
		}
	}
	delete sceneController;
	sceneController = nullptr;
}

void Application::Terminate()
{
	Physics::GetInstance().Reset();
	EffekseerManager::GetInstance().End();
	UIManager::GetInstance().End();
	SoundManager::GetInstance().End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

