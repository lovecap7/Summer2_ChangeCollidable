#include "MathSub.h"

int MathSub::ClampInt(int value, int min, int max)
{
    //”ÍˆÍ“à‚Éû‚Ü‚Á‚Ä‚¢‚é‚È‚ç‚»‚Ì‚Ü‚Ü•Ô‚·
    int clampValue = value;
    //Å¬‚æ‚è¬‚³‚¢‚È‚çÅ¬‚É‡‚í‚¹‚é
    if (clampValue < min)
    {
        clampValue = min;
    }
    //Å‘å‚æ‚è‘å‚«‚¢‚È‚çÅ‘å‚É‡‚í‚¹‚é
    if (clampValue > max)
    {
        clampValue = max;
    }
    return clampValue;
}

float MathSub::ClampFloat(float value, float min, float max)
{
    //”ÍˆÍ“à‚Éû‚Ü‚Á‚Ä‚¢‚é‚È‚ç‚»‚Ì‚Ü‚Ü•Ô‚·
    float clampValue = value;
    //Å¬‚æ‚è¬‚³‚¢‚È‚çÅ¬‚É‡‚í‚¹‚é
    if (clampValue < min)
    {
        clampValue = min;
    }
    //Å‘å‚æ‚è‘å‚«‚¢‚È‚çÅ‘å‚É‡‚í‚¹‚é
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

float MathSub::Min(float a, float b)
{
    float min = a;
    if (a > b)
    {
        min = b;
    }
    return min;
}

float MathSub::Max(float a, float b)
{
    float max = a;
    if (a < b)
    {
        max = b;
    }
    return max;
}
