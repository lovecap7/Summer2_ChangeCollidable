#include "MathSub.h"

int MathSub::ClampInt(int value, int min, int max)
{
    //�͈͓��Ɏ��܂��Ă���Ȃ炻�̂܂ܕԂ�
    int clampValue = value;
    //�ŏ���菬�����Ȃ�ŏ��ɍ��킹��
    if (clampValue < min)
    {
        clampValue = min;
    }
    //�ő���傫���Ȃ�ő�ɍ��킹��
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float MathSub::ClampFloat(float value, float min, float max)
{
    //�͈͓��Ɏ��܂��Ă���Ȃ炻�̂܂ܕԂ�
    float clampValue = value;
    //�ŏ���菬�����Ȃ�ŏ��ɍ��킹��
    if (clampValue < min)
    {
        clampValue = min;
    }
    //�ő���傫���Ȃ�ő�ɍ��킹��
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float MathSub::Lerp(float start, float end, float t)
{
    float result = start * (1.0f - t) + end * t;
    return result;
}
