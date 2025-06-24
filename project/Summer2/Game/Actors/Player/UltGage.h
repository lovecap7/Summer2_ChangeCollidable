#pragma once
class UltGage
{
public:
	UltGage(int max);
	~UltGage();
	//今のゲージ量を返す
	int GetUltGage() { return m_ultGage; };
	//ゲージを加算
	void AddUltGage(int add);
	//ゲージリセット
	void ResetUltGage() { m_ultGage = 0; };
	//必殺技ゲージがマックスならtrue
	bool IsMaxUlt() { return m_ultGage >= m_maxUltGage; };
	//ゲージの最大値
	int GetMaxUltGage() { return m_maxUltGage; };
	//加算される予定の値をセット
	void SetPendingUltGage(int pendingUltGage) { m_pendingUltGauge = pendingUltGage; };
	//予定値の反映
	void AddPedingUltGage() { AddUltGage(m_pendingUltGauge); };
private:
	int m_ultGage;
	int m_maxUltGage;
	//加算される予定の値(攻撃が当たった時に加算される)
	int m_pendingUltGauge;
};

