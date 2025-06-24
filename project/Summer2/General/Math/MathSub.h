#pragma once
class MathSub
{
public:
    /// <summary>
    /// ����l�����͈͓��Ɏ��܂�悤�ɃN�����v����
    /// </summary>
    /// <param name="value">�N�����v����l</param>
    /// <param name="min">�ŏ�</param>
    /// <param name="max">�ő�</param>
    /// <returns>int�^</returns>
    static int ClampInt(int value, int min, int max);
    /// <summary>
    /// ����l�����͈͓��Ɏ��܂�悤�ɃN�����v����
    /// </summary>
    /// <param name="value">�N�����v����l</param>
    /// <param name="min">�ŏ�</param>
    /// <param name="max">�ő�</param>
    /// <returns>float�^</returns>
    static float ClampFloat(float value, float min, float max);
    /// <summary>
    /// ���`���
    /// </summary>
    /// <param name="start">�ŏ�</param>
    /// <param name="end">�Ō�</param>
    /// <param name="t">��Ԃ̊���</param>
    /// <returns></returns>
    static float Lerp(float start, float end, float t);
};

