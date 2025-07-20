#include "StringUtil.h"

TCHAR* ToTCHAR(const char* c)
{
	// char* → TCHAR* に変換
	TCHAR cT[256];
	MultiByteToWideChar(CP_ACP, 0, c, -1, cT, 256);
	return cT;
}

std::wstring InsertNewLines(const std::wstring& str, int maxLength)
{
	std::wstring newStr;
	//あらかじめ必要なメモリを確保(メモリ最適化)
	newStr.reserve(str.size() + (str.size() / maxLength));
	//文字を数えていく
	size_t count = 0;
	for (auto c : str)
	{
		//文字を入れていく
		newStr += c;
		//改行が挟まった時カウントを0に
		if (c == '\n')
		{
			count = 0;
			continue;
		}
		//カウント
		++count;
		//一行の最大文字数を超えたら
		if (count > maxLength)
		{
			//改行
			newStr += '\n';
			//リセット
			count = 0;
		}
	}
	return newStr;
}

int WStringLineNum(const std::wstring& str)
{
	size_t count = 0;
	for (char c : str) {
		if (c == '\n') {
			count++;
		}
	}
	return static_cast<int>(count);
}
