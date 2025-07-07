#include "TrackActorEffect.h"
#include "../../Game/Actors/Actor.h"

TrackActorEffect::TrackActorEffect(int playHandle, std::weak_ptr<Actor> target):
	MyEffect(playHandle, target.lock()->GetPos()),
	m_target(target)
{
}

TrackActorEffect::~TrackActorEffect()
{
}

void TrackActorEffect::Update()
{
	//�^�[�Q�b�g�����Ȃ��Ȃ�폜
	if (m_target.expired())
	{
		Delete();
		return;
	}
	//�ړ�
	SetPos(m_target.lock()->GetPos());
	//�X�V
	MyEffect::Update();
}
