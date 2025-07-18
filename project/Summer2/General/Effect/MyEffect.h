#pragma once
#include "../Math/MyMath.h"
class MyEffect
{
public:
	MyEffect(int playHandle, Vector3 pos);
	virtual ~MyEffect();
	virtual void Update();			//更新
	virtual void End();				//終了処理
	void Play();					//再生
	void Stop();					//停止
	void SetPlaySpeed(float speed);	//再生速度
	void Delete();					//削除
	bool IsDelete() const { return m_isDelete; };
	//セッター
	void SetPos(Vector3 pos);		//座標
	void SetRot(Vector3 rot);		//回転
	void SetScale(Vector3 scale);	//拡大
	void LookAt(Vector3 dir);		//向きたい方向

	//指定フレーム再生
	void SpecificFrame(int frame);
protected:
	//再生ハンドル
	int m_playHandle;
	//座標
	Vector3 m_pos;
	//回転
	Vector3 m_rot;
	//拡大
	Vector3 m_scale;
	//再生速度
	float m_playSpeed;
	//削除フラグ
	bool m_isDelete;
	//指定フレーム後に削除
	int m_specificFrame;
	bool m_isSpecificFrame;
	//エフェクトの向き(Z方向)
	Vector3 m_dir;
};

