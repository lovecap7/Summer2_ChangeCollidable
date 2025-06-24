#pragma once
#include <memory>
class Collidable;
class CollisionChecker
{
public:
    CollisionChecker();
    virtual ~CollisionChecker() {};

    /// <summary>
    /// 球と球の当たり判定
    /// </summary>
    /// <param name="actorA">球</param>
    /// <param name="actorB">球</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollSS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// カプセルと球の当たり判定(引数に注意してください)
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">球</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollCS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// カプセルとカプセルの当たり判定
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">カプセル</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// カプセルとカプセルの当たり判定
    /// </summary>
      /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">カプセル</param>
    /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollCCVerDxLib(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// 球とポリゴンの当たり判定
    /// </summary>
    /// <param name="actorA">球</param>
    /// <param name="actorB">ポリゴン</param>
     /// <returns>当たってる true : 当たってない false</returns>
    bool CheckCollSP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// カプセルとポリゴンの当たり判定
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">ポリゴン</param>
    /// <returns></returns>
    bool CheckCollCP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);
private:
    /// <summary>
    /// カプセル同士で平行の場合の当たり判定処理
    /// </summary>
    /// <param name="actorA">カプセル</param>
    /// <param name="actorB">カプセル</param>
    /// <returns></returns>
    bool ParallelCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);
};

