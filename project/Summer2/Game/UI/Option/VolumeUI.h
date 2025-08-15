#pragma once
#include "../UIBase.h"
#include "../../../General/Math/MyMath.h"
class VolumeUI :
    public UIBase
{
public:
    VolumeUI(float posY);
    ~VolumeUI();
    void Update()override;
    void Draw() const override;
    void SetVolume(int volume) { m_volume = volume; };
private:
    int m_volume;
    Vector2 m_pos;
};

