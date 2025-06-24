#include "Application.h"
#include <DxLib.h>
#include "../General/game.h"
#include "../Scene/SceneController.h"
#include  "../General/Input.h"

Application& Application::GetInstance()
{
    //この時点でメモリが確保されてアプリ終了まで残る
    static Application app;
    return app;
}

bool Application::Init()
{
    //フルスクリーンでなく、ウィンドウモードで開くようにする
    //こういった関数はウィンドウが開く前に(Dxlib.Init()の前)に処理しておく必要がある
	ChangeWindowMode(Game::kDefaultWindowMode);
	//画面サイズ変更
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorBitNum);
	//ゲームタイトル
	SetWindowText("タイトル");
	//ゲームアイコン
	//SetWindowIconID(IDI_ICON1);

	//ここに書くと一瞬フルスクリーンになるので書かない
	//ChangeWindowMode(true);
	//SetGraphMode(1280, 720, 32);


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

    return true;
}

void Application::Run()
{
	//アプリケーション以外はここで宣言と初期化
	SceneController* sceneController = new SceneController();
	//コントローラー
	Input* input = new Input();
	input->Init();

	//ゲームループ
	while (ProcessMessage() != -1) // Windowsが行う処理を待つ
	{
		//今回のループが始まった時間を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		//画面全体をクリア
		ClearDrawScreen();

		//ここにゲームの処理を書く
		input->Update();
		sceneController->Update(*input);
		sceneController->Draw();

		//画面の切り替わりを待つ必要がある
		ScreenFlip();//1/60秒経過するまで待つ

		//FPSを60に固定
		while (GetNowHiPerformanceCount() - time < 16667);
		{

		}

		//ESCキーで終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			//消す
			delete input;
			input = nullptr;
			sceneController = nullptr;
			break;
		}
	}
	//消す
	delete input;
	input = nullptr;
	delete sceneController;
	sceneController = nullptr;
}

void Application::Terminate()
{
	DxLib_End();				// ＤＸライブラリ使用の終了処理
}

