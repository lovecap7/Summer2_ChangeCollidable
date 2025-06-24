#pragma once

/// <summary>
/// 当たり判定の形によって処理が変わるので
/// お互いがどの形の当たり判定をしているのかを
/// 先にチェックする際に使う
/// </summary>
enum class Shape
{
	Sphere,
	Capsule,
	Box,
	Polygon
};

/// <summary>
/// 当たり判定の形に関する基底クラス
/// </summary>
class ColliderBase abstract
{
public:
	ColliderBase(Shape kind);
	virtual ~ColliderBase() {};

	/// <summary>
	/// 当たり判定の形を取得
	/// </summary>
	/// <returns>当たり判定の形</returns>
	Shape GetShape()const { return m_kind; }
private:
	//当たり判定の形
	Shape m_kind;
};

