#include "FixNextPosition.h"
#include "Collidable.h"
#include "../Rigidbody.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "PolygonCollider.h"
#include <vector>

namespace
{
	//押し戻しの値に足して密着するのを防ぐ
	constexpr float kOverlapGap = 1.0f;
	constexpr float kCheckUnder = -800.0f;
	constexpr float kCheckTop = 800.0f;
}

FixNextPosition::FixNextPosition() :
	m_wallNum(0),
	m_floorAndRoofNum(0),
	m_wall{ nullptr },
	m_floorAndRoof{ nullptr }
{
}

FixNextPosition::~FixNextPosition()
{
}

void FixNextPosition::FixNextPos(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//衝突しているオブジェクトの形状を取得
	auto collAShape = collA->m_collisionData->m_shape;
	auto collBShape = collB->m_collisionData->m_shape;
	//球と
	if (collAShape == Shape::Sphere)
	{
		//球
		if (collBShape == Shape::Sphere)
		{
			//ベクトルを補正する
			FixNextPosSS(collA, collB);
		}
		//カプセル
		else if (collBShape == Shape::Capsule)
		{
			//ベクトルを補正する
			FixNextPosCS(collB, collA);
		}
		//ポリゴン
		else if (collBShape == Shape::Polygon)
		{
			//ベクトルを補正する
			FixNextPosSP(collA, collB);
		}
	}
	//カプセルと
	else if (collAShape == Shape::Capsule)
	{
		//球
		if (collBShape == Shape::Sphere)
		{
			//ベクトルを補正する
			FixNextPosCS(collA, collB);
		}
		//カプセル
		else if (collBShape == Shape::Capsule)
		{
			//ベクトルを補正する
			FixNextPosCC(collA, collB);
		}
		//ポリゴン
		else if (collBShape == Shape::Polygon)
		{
			//ベクトルを補正する
			FixNextPosCP(collA, collB);
		}
	}
	//ポリゴンと
	else if (collAShape == Shape::Polygon)
	{
		//球
		if (collBShape == Shape::Sphere)
		{
			//ベクトルを補正する
			FixNextPosSP(collB, collA);
		}
		//カプセル
		else if (collBShape == Shape::Capsule)
		{
			//ベクトルを補正する
			FixNextPosCP(collB, collA);
		}
	}
}

void FixNextPosition::FixNextPosSS(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//優先度
	auto priorityA = collA->m_priority;
	auto priorityB = collB->m_priority;
	//お互い動かないオブジェクトなら衝突しない
	if (priorityA == Priority::Static &&
		priorityB == Priority::Static)return;

	//リジッドボディ
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//AからBへのベクトル
	Vector3 aToB = rbB->GetNextPos() - rbA->GetNextPos();
	//最短距離
	float shortDis = std::dynamic_pointer_cast<SphereCollider> (collA->m_collisionData)->GetRadius() + std::dynamic_pointer_cast<SphereCollider> (collB->m_collisionData)->GetRadius();
	//どのくらい重ねっているか
	float overlap = shortDis - aToB.Magnitude();
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	//優先度から動かすほうを決める
	if (priorityA > priorityB)
	{
		rbB->AddVec(aToB.Normalize() * overlap);
	}
	else if (priorityA < priorityB)
	{
		rbA->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		rbA->AddVec(aToB.Normalize() * -overlap / 2.0f);
		rbB->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void FixNextPosition::FixNextPosSP(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//コリジョンデータ
	auto collDataA = std::dynamic_pointer_cast<SphereCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<PolygonCollider>(collB->m_collisionData);
	//リジッドボディ
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;
	
	//当たったポリゴンの情報
	auto& hitDim = collDataB->GetHitDim();

	//お互い動かないオブジェクトなら衝突しない(ポリゴンはスタティックなので片方がスタティックなら)
	if (collA->m_priority == Priority::Static)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		DxLib::MV1CollResultPolyDimTerminate(hitDim);
		return;
	}

	//球の座標
	Position3 nextPos = rbA->GetNextPos();//移動後

	//床ポリゴンと壁ポリゴンに分ける
	AnalyzeWallAndFloor(hitDim, nextPos);
	//床と当たったなら
	if (m_floorAndRoofNum > 0)
	{
		//補正するベクトルを返す
		Vector3 overlapVec = OverlapVecSphereAndPoly(m_floorAndRoofNum, nextPos, *m_floorAndRoof, collDataA->m_radius);
	
		//ポリゴンは固定(static)なので球のみ動かす
		rbA->AddVec(overlapVec);
		//修正方向が上向きなら床
		if (overlapVec.y > 0)
		{
			//床に当たっているので
			collA->SetIsFloor(true);
		}
	}

	//壁と当たっているなら
	if(m_wallNum > 0)
	{
		//壁に当たっているので
		collA->SetIsWall(true);

		//補正するベクトルを返す
		Vector3 overlapVec = OverlapVecSphereAndPoly(m_wallNum, nextPos, *m_wall, collDataA->m_radius);
		
		//ポリゴンは固定(static)なので球のみ動かす
		rbA->AddVec(overlapVec);
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	DxLib::MV1CollResultPolyDimTerminate(hitDim);
}

void FixNextPosition::FixNextPosCC(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//優先度
	auto priorityA = collA->m_priority;
	auto priorityB = collB->m_priority;
	//お互い動かないオブジェクトなら衝突しない
	if (priorityA == Priority::Static &&
		priorityB == Priority::Static)return;

	//カプセルの押し戻しはそれぞれの当たったポイントから計算します

	//コライダーデータ
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<CapsuleCollider>(collB->m_collisionData);
	//リジッドボディ
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//AからBへのベクトル
	Vector3 aToB = collDataB->GetNearPos() - collDataA->GetNearPos();
	//最短距離
	float shortDis = collDataA->m_radius + collDataB->m_radius;
	//どのくらい重ねっているか
	float overlap = shortDis - aToB.Magnitude();
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	//横方向にだけ動かしたいので
	aToB.y = 0.0f;

	//動かす物体とそうじゃない物体とで処理を分ける
	if (priorityA > priorityB)
	{
		rbB->AddVec(aToB.Normalize() * overlap);
	}
	else if (priorityA < priorityB)
	{
		rbA->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		rbA->AddVec(aToB.Normalize() * -overlap / 2.0f);
		rbB->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void FixNextPosition::FixNextPosCS(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//優先度
	auto priorityA = collA->m_priority;
	auto priorityB = collB->m_priority;
	//お互い動かないオブジェクトなら衝突しない
	if (priorityA == Priority::Static &&
		priorityB == Priority::Static)return;
	//コライダーデータ
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<SphereCollider>(collB->m_collisionData);
	//リジッドボディ
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//AからBへのベクトル
	Vector3 aToB = rbB->GetNextPos() - collDataA->GetNearPos();

	//最短距離
	float shortDis = collDataB->m_radius + collDataA->GetRadius();
	//どのくらい重ねっているか
	float overlap = shortDis - aToB.Magnitude();
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	//動かす物体とそうじゃない物体とで処理を分ける
	if (priorityA > priorityB)
	{
		rbB->AddVec(aToB.Normalize() * overlap);
	}
	else if (priorityA < priorityB)
	{
		rbA->AddVec(aToB.Normalize() * -overlap);
	}
	else
	{
		rbA->AddVec(aToB.Normalize() * -overlap / 2.0f);
		rbB->AddVec(aToB.Normalize() * overlap / 2.0f);
	}
}

void FixNextPosition::FixNextPosCP(const std::shared_ptr<Collidable> collA, const std::shared_ptr<Collidable> collB)
{
	//コライダーデータ
	auto collDataA = std::dynamic_pointer_cast<CapsuleCollider>(collA->m_collisionData);
	auto collDataB = std::dynamic_pointer_cast<PolygonCollider>(collB->m_collisionData);
	//リジッドボディ
	auto rbA = collA->m_rb;
	auto rbB = collB->m_rb;

	//当たったポリゴンの情報
	auto& hitDim = collDataB->GetHitDim();
	//お互い動かないオブジェクトなら衝突しない(ポリゴンはスタティックなので片方がスタティックなら)
	if (collA->m_priority == Priority::Static)
	{
		// 検出したプレイヤーの周囲のポリゴン情報を開放する
		DxLib::MV1CollResultPolyDimTerminate(hitDim);
		return;
	}

	//カプセルの頭座標と足座標
	Position3 headPos = collDataA->GetNextEndPos(rbA->GetVec());//移動後
	Position3 legPos = rbA->GetNextPos();//移動後
	//頭より足のほうが低い位置にあるなら入れ替える
	if (headPos.y < legPos.y)
	{
		Position3 temp = legPos;
		legPos = headPos;
		headPos = temp;
	}

	//床ポリゴンと壁ポリゴンに分ける
	AnalyzeWallAndFloor(hitDim, legPos);
	//床と当たったなら
	if (m_floorAndRoofNum > 0)
	{
		//ジャンプしているなら
		if (collA->m_collState == CollisionState::Jump)
		{
			//天井に当たった処理
			HitRoofCP(collA, headPos, m_floorAndRoofNum, *m_floorAndRoof, collDataA->m_radius);
		}
		else
		{
			//床の高さに合わせる
			if (HitFloorCP(collA, legPos, m_floorAndRoofNum, *m_floorAndRoof, collDataA->m_radius))
			{
				//床に当たっているので
				collA->SetIsFloor(true);
			}
		}
	}

	//壁と当たっているなら
	if (m_wallNum > 0)
	{
		//壁に当たっているので
		collA->SetIsWall(true);

		//補正するベクトルを返す
		Vector3 overlapVec = HitWallCP(headPos, legPos, m_wallNum, *m_wall, collDataA->GetRadius());
		
		//ポリゴンは固定(static)なので球のみ動かす
		rbA->AddVec(overlapVec);
	}

	// 検出したプレイヤーの周囲のポリゴン情報を開放する
	DxLib::MV1CollResultPolyDimTerminate(hitDim);

}

void FixNextPosition::AnalyzeWallAndFloor(MV1_COLL_RESULT_POLY_DIM hitDim, const Vector3& nextPos)
{
	//壁ポリゴンと床ポリゴンの数を初期化する
	m_wallNum = 0;
	m_floorAndRoofNum = 0;

	//検出されたポリゴンの数だけ繰り返す
	for (int i = 0; i < hitDim.HitNum;++i)
	{
		//XZ平面に垂直かどうかはポリゴンの法線のY成分が0に近いかどうかで判断する
		if (hitDim.Dim[i].Normal.y < 0.1f && hitDim.Dim[i].Normal.y > -0.1f)
		{
			//壁ポリゴンと判断された場合でも、プレイヤーのY座標＋1.0fより高いポリゴンのみ当たり判定を行う
			//段さで突っかかるのを防ぐため
			if (hitDim.Dim[i].Position[0].y > nextPos.y + 1.0f ||
				hitDim.Dim[i].Position[1].y > nextPos.y + 1.0f ||
				hitDim.Dim[i].Position[2].y > nextPos.y + 1.0f)
			{
				//ポリゴンの数が列挙できる限界数に達していなかったらポリゴンを配列に保存する
				if (m_wallNum < kMaxHitPolygon)
				{
					//ポリゴンの構造体のアドレスを壁ポリゴン配列に保存
					m_wall[m_wallNum] = &hitDim.Dim[i];
					++m_wallNum;
				}
			}
		}
		//床ポリゴン
		else
		{
			//ポリゴンの数が列挙できる限界数に達していなかったらポリゴン配列に保存
			if (m_floorAndRoofNum < kMaxHitPolygon)
			{
				//ポリゴンの構造体のアドレスを床ポリゴン配列に保存
				m_floorAndRoof[m_floorAndRoofNum] = &hitDim.Dim[i];
				++m_floorAndRoofNum;
			}
		}
	}
}

Vector3 FixNextPosition::OverlapVecSphereAndPoly(int hitNum ,const Vector3& nextPos,MV1_COLL_RESULT_POLY* dim ,float shortDis)
{
	//垂線を下して近い点を探して最短距離を求める
	float hitShortDis = 0;//最短距離
	//法線
	Vector3 nom = {};
	for (int i = 0; i < hitNum; ++i)
	{
		//内積と法線ベクトルから当たってる座標を求める
		VECTOR bToA = VSub(nextPos.ToDxLibVector(), dim[i].Position[0]);
		float dot = VDot(dim[i].Normal, bToA);

		//ポリゴンと当たったオブジェクトが法線方向にいるなら向きを反転
		if ((bToA.y > 0 && dim[i].Normal.y > 0) || (bToA.y < 0 && dim[i].Normal.y < 0))
		{
			dot *= -1;
		}
		//当たった座標
		VECTOR hitPos = VAdd(VScale(dim[i].Normal, dot), nextPos.ToDxLibVector());
		//距離
		float dis = VSize(VSub(hitPos, nextPos.ToDxLibVector()));
		//初回または前回より距離が短いなら
		if (i <= 0 || hitShortDis > dis)
		{
			//現状の最短
			hitShortDis = dis;
			//法線
			nom = Vector3{ dim[i].Normal.x,dim[i].Normal.y ,dim[i].Normal.z };
		}
	}
	//押し戻し
	//どれくらい押し戻すか
	float overlap = shortDis - hitShortDis;
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;

	return nom.Normalize() * overlap;
}

Vector3 FixNextPosition::HitWallCP(const Vector3& headPos, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//垂線を下して近い点を探して最短距離を求める
	float hitShortDis = shortDis;//最短距離
	//法線
	Vector3 nom = {};
	for (int i = 0; i < hitNum; ++i)
	{
		//最短距離の2乗を返す
		float dis = Segment_Triangle_MinLength_Square(headPos.ToDxLibVector(), legPos.ToDxLibVector(), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);
		//平方根を返す
		dis = sqrtf(dis);
		//初回または前回より距離が短いなら
		if (hitShortDis > dis)
		{
			//現状の最短
			hitShortDis = dis;
			//法線
			nom = Vector3{ dim[i].Normal.x,0.0f ,dim[i].Normal.z };
		}
	}
	//押し戻し
	//どれくらい押し戻すか
	float overlap = shortDis - hitShortDis;
	overlap = MathSub::ClampFloat(overlap, 0, shortDis);
	overlap += kOverlapGap;
	//正規化
	if (nom.Magnitude() != 0.0f)
	{
		nom = nom.Normalize();
	}
	return nom * overlap;
}


bool FixNextPosition::HitFloorCP(const std::shared_ptr<Collidable> coll, const Vector3& legPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//リジッドボディ
	auto rb = coll->m_rb;

	//垂線を下して近い点を探して最短距離を求める
	float hitShortDis = shortDis;//最短距離
	//当たった中で足元に一番近いY座標に合わせる
	float lowHitPosY = rb->GetPos().y;
	//床と当たったか
	bool hitFloor = false;
	for (int i = 0; i < hitNum; ++i)
	{
		//下向きの法線ベクトルなら飛ばす
		if (dim[i].Normal.y < 0.0f)continue;
		// 足の下にポリゴンがあるかをチェック
		 HITRESULT_LINE lineResult = HitCheck_Line_Triangle(legPos.ToDxLibVector(), VAdd(legPos.ToDxLibVector(), VGet(0.0f, kCheckUnder, 0.0f)), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);

		if (lineResult.HitFlag)
		{
			hitFloor = true;
			 //距離
			 float dis = VSize(VSub(lineResult.Position, legPos.ToDxLibVector()));
			 //初回または前回より距離が短いなら
			 if (hitShortDis > dis)
			 {
				 //現状の最短
				 hitShortDis = dis;
				 lowHitPosY = lineResult.Position.y;
			 }
		}
	}
	//当たったいるなら
	if (hitFloor)
	{
		//床の高さに合わせる
		lowHitPosY += shortDis + kOverlapGap;
		rb->SetPosY(lowHitPosY);
		rb->SetVecY(0.0f);
	}
	return hitFloor;
}

void FixNextPosition::HitRoofCP(const std::shared_ptr<Collidable> coll, const Vector3& headPos, int hitNum, MV1_COLL_RESULT_POLY* dim, float shortDis)
{
	//リジッドボディ
	auto rb = coll->m_rb;
	//垂線を下して近い点を探して最短距離を求める
	float hitShortDis = shortDis;//最短距離
	//当たった中で足元に一番近いY座標に合わせる
	float lowHitPosY = rb->GetPos().y;
	//天井と当たったか
	bool hitRoof = false;
	for (int i = 0; i < hitNum; ++i)
	{
		//上向きの法線ベクトルなら飛ばす
		if (dim[i].Normal.y > 0.0f)continue;
		// 頭の上にポリゴンがあるかをチェック
		HITRESULT_LINE lineResult = HitCheck_Line_Triangle(headPos.ToDxLibVector(), VAdd(headPos.ToDxLibVector(), VGet(0.0f, kCheckTop, 0.0f)), dim[i].Position[0], dim[i].Position[1], dim[i].Position[2]);

		if (lineResult.HitFlag)
		{
			hitRoof = true;
			//距離
			float dis = VSize(VSub(lineResult.Position, headPos.ToDxLibVector()));
			//初回または前回より距離が短いなら
			if (hitShortDis > dis)
			{
				//現状の最短
				hitShortDis = dis;
			}
		}
	}
	//当たったいるなら
	if (hitRoof)
	{
		//押し戻し
		//どれくらい押し戻すか
		float overlap = shortDis - hitShortDis;
		overlap = MathSub::ClampFloat(overlap, 0, shortDis);
		overlap += kOverlapGap;
		//法線
		Vector3 nom = { 0.0f,-1.0f,0.0f };
		//力を与える
		rb->AddVec(nom * overlap);
	}
}


