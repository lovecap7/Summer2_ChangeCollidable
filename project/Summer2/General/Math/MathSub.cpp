#include "MathSub.h"

int MathSub::ClampInt(int value, int min, int max)
{
    //範囲内に収まっているならそのまま返す
    int clampValue = value;
    //最小より小さいなら最小に合わせる
    if (clampValue < min)
    {
        clampValue = min;
    }
    //最大より大きいなら最大に合わせる
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float MathSub::ClampFloat(float value, float min, float max)
{
    //範囲内に収まっているならそのまま返す
    float clampValue = value;
    //最小より小さいなら最小に合わせる
    if (clampValue < min)
    {
        clampValue = min;
    }
    //最大より大きいなら最大に合わせる
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
