#include <DxLib.h>
#include "Application.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if _DEBUG
	AllocConsole();                                        // �R���\�[��
	FILE* out = 0; freopen_s(&out, "CON", "w", stdout); // stdout
	FILE* in = 0; freopen_s(&in, "CON", "r", stdin);   // stdin
#endif
	//C++��static�����o�̌Ăяo���́@::�@�Ȃ̂Œ���
	Application& app = Application::GetInstance();
	if (!app.Init())
	{
		return -1;
	}
	app.Run();
	app.Terminate();

#if _DEBUG
	fclose(out); fclose(in); FreeConsole();//�R���\�[�����
#endif
	return 0;				// �\�t�g�̏I�� 
}