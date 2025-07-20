#pragma once
#include <DxLib.h>
#include <string>
/// <summary>
/// char -> TCHA
/// </summary>
/// <param name="c"></param>
/// <returns></returns>
TCHAR* ToTCHAR(const char* c);

/// <summary>
/// •¶š”‚ª‚ ‚é”‚ğ’´‚¦‚é‚½‚Ñ‚É‰üs‚ğ‚Í‚³‚İA‚»‚Ì•¶š—ñ‚ğ•Ô‚·
/// </summary>
/// <param name="str"></param>
/// <param name="maxLength"></param>
/// <returns></returns>
std::wstring InsertNewLines(const std::wstring& str, int maxLength);

/// <summary>
/// s‚Ì”
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
int WStringLineNum(const std::wstring& str);
