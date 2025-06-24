#pragma once
#include <memory>
class Collidable;
class CollisionChecker
{
public:
    CollisionChecker();
    virtual ~CollisionChecker() {};

    /// <summary>
    /// ���Ƌ��̓����蔻��
    /// </summary>
    /// <param name="actorA">��</param>
    /// <param name="actorB">��</param>
    /// <returns>�������Ă� true : �������ĂȂ� false</returns>
    bool CheckCollSS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// �J�v�Z���Ƌ��̓����蔻��(�����ɒ��ӂ��Ă�������)
    /// </summary>
    /// <param name="actorA">�J�v�Z��</param>
    /// <param name="actorB">��</param>
    /// <returns>�������Ă� true : �������ĂȂ� false</returns>
    bool CheckCollCS(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// �J�v�Z���ƃJ�v�Z���̓����蔻��
    /// </summary>
    /// <param name="actorA">�J�v�Z��</param>
    /// <param name="actorB">�J�v�Z��</param>
    /// <returns>�������Ă� true : �������ĂȂ� false</returns>
    bool CheckCollCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// �J�v�Z���ƃJ�v�Z���̓����蔻��
    /// </summary>
      /// </summary>
    /// <param name="actorA">�J�v�Z��</param>
    /// <param name="actorB">�J�v�Z��</param>
    /// <returns>�������Ă� true : �������ĂȂ� false</returns>
    bool CheckCollCCVerDxLib(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// ���ƃ|���S���̓����蔻��
    /// </summary>
    /// <param name="actorA">��</param>
    /// <param name="actorB">�|���S��</param>
     /// <returns>�������Ă� true : �������ĂȂ� false</returns>
    bool CheckCollSP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);

    /// <summary>
    /// �J�v�Z���ƃ|���S���̓����蔻��
    /// </summary>
    /// <param name="actorA">�J�v�Z��</param>
    /// <param name="actorB">�|���S��</param>
    /// <returns></returns>
    bool CheckCollCP(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);
private:
    /// <summary>
    /// �J�v�Z�����m�ŕ��s�̏ꍇ�̓����蔻�菈��
    /// </summary>
    /// <param name="actorA">�J�v�Z��</param>
    /// <param name="actorB">�J�v�Z��</param>
    /// <returns></returns>
    bool ParallelCC(const std::shared_ptr<Collidable>& collA, const std::shared_ptr<Collidable>& collB);
};

