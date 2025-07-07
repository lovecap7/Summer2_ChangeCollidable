#pragma once
#include "MyEffect.h"
#include <memory>
class Actor;
//������̍��W�ɍ��킹�Ĉړ�(�ǐ�)����N���X
class TrackActorEffect :
    public MyEffect
{
public:
	TrackActorEffect(int playHandle,std::weak_ptr<Actor> target);
	~TrackActorEffect();
	void Update() override;			//�X�V
private:
	//�ǐՃ^�[�Q�b�g
	std::weak_ptr<Actor> m_target;
};

