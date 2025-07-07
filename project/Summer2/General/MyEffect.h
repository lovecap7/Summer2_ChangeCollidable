#pragma once
#include "Math/MyMath.h"
class MyEffect
{
public:
	MyEffect(int playHandle, Vector3 pos);
	~MyEffect();
	void Update();					//更新
	void Play();					//再生
	void Stop();					//停止
	void SetPlaySpeed(float speed);	//再生速度
	void Delete();					//削除

	void SetPos(Vector3 pos);		//座標
	void SetRot(Vector3 rot);		//回転
	void SetScale(Vector3 scale);		//拡大
private:
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
};

