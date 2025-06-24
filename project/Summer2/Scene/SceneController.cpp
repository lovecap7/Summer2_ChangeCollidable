#include "SceneController.h"
#include <memory>
#include<cassert>
//継承元の関数も認識してくれる
#include "TitleScene.h"
#if _DEBUG
#include "DebugScene.h"
#endif

SceneController::SceneController()
{
#if _DEBUG
	//一番最初のシーンだけは割り当てる
	//自分自身のインスタンスを渡してあげる
	ChangeScene(std::make_shared<DebugScene>(*this));
#else
	//一番最初のシーンだけは割り当てる
	//自分自身のインスタンスを渡してあげる
	ChangeScene(std::make_shared<TitleScene>(*this));
#endif

}

void SceneController::Update(Input& input)
{
	//最後にプッシュ(入れた)シーンのみ更新処理を行う(他のシーンは更新はストップ)
	m_scenes.back()->Update(input);
}

void SceneController::Draw()
{
	//古いシーンから描画されるので最後に入れたシーンが前に描画される
	for (auto& scenes : m_scenes)
	{
		scenes->Draw();
	}
}

void SceneController::ChangeScene(std::shared_ptr<SceneBase> scene)
{
	//空(最初の処理)
	if (m_scenes.empty())
	{
		m_scenes.emplace_back(scene);
	}
	else
	{
		//先頭のシーンが切り替わる
		m_scenes.back() = scene;
	}
	//初期化処理
	m_scenes.back()->Init();
}

void SceneController::ChangeBaseScene(std::shared_ptr<SceneBase> scene)
{
	//空の場合エラー
	assert(!m_scenes.empty());
	m_scenes.front() = scene;
}

void SceneController::PushScene(std::shared_ptr<SceneBase> scene)
{
	m_scenes.emplace_back(scene);
}

void SceneController::PopScene(std::shared_ptr<SceneBase> scene)
{
	//実行するシーンがなくなるので許可しない
	if (m_scenes.size() == 1)return;
	m_scenes.pop_back();//末尾を取り除く
}
