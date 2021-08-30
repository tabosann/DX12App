#pragma once

#include<tchar.h>	//�}���`�o�C�g�������A���C�h�������A�g�p����蔻�f
#include<Windows.h>	//OutputDebugString�}�N��
#include<crtdbg.h>	//_ASSERT_EXPR�}�N��

//�֗��ȃf�o�b�O�@�\���A�ϒ�������������悤�Ɋg���B
#ifdef _DEBUG

	//�ϒ�������������OutputDebugString�}�N��
	#ifndef OutputDebugStringEx
		#define OutputDebugStringEx(format, ...)\
				{														\
					int size = _sctprintf(format, __VA_ARGS__) + 1;		\
					if(TCHAR* tstr = new TCHAR[size])					\
					{													\
						_stprintf_s(tstr, size, format, __VA_ARGS__);	\
						OutputDebugString(tstr);						\
						delete[] tstr;									\
					}													\
				}
	#endif

	//�ϒ�������������_ASSERT_EXPR�}�N��
	#ifndef _ASSERT_EXPR_EX
		#define _ASSERT_EXPR_EX(expr, format, ...)\
				{														\
					int size = _sctprintf(format, __VA_ARGS__) + 1;		\
					if(TCHAR* tstr = new TCHAR[size])					\
					{													\
						_stprintf_s(tstr, size, format, __VA_ARGS__);	\
						_ASSERT_EXPR(expr, tstr);						\
						delete[] tstr;									\
					}													\
				}
	#endif

#else

	//�f�o�b�O���[�h�łȂ��Ȃ�A�������Ȃ��悤�ɂ���(����ۂ��`)�B

	#ifndef OutputDebugStringEx
		#define OutputDebugStringEx(str, ...)
	#endif

	#ifndef _ASSERT_EXPR_EX
		#define _ASSERT_EXPR_EX(expr, str, ...)
	#endif

#endif

#ifndef _ASSERT_BASE_EX
	#define _ASSERT_BASE_EX _ASSERT_EXPR_EX
#endif

//���������[�N�𒲂ׂ邽�߂̃f�t�H���g�ݒ�
#ifndef _CRTDBG_DEFAULT_CHECK_MEMORYLEAK_DF
	#define _CRTDBG_DEFAULT_CHECK_MEMORYLEAK_DF _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF
#endif

namespace DebugTools
{
	// �v���O�����J�n����I���܂ł̊ԂŁA���������[�N���������������ׂ�B
	// �������Ă����ꍇ�́A�f�o�b�O�E�B���h�E�ɂ��̎|��\������B
	// NOTE : ��L�̐����̓f�t�H���g�ݒ�̏ꍇ���w��
	void DebugMemoryLeak(int addFlag = NULL);

	// WindowsAPI�̒��߂̃G���[�R�[�h���E�B���h�E��ʂŐ������A���̃G���[�R�[�h�Ńv���O�������I������B
	// NOTE : �f�o�b�O���[�h�łȂ��Ȃ�A���̊֐��͂Ȃɂ����Ȃ��B
	void ErrorExit(LPTSTR lpszFunction);
}
namespace dbg = DebugTools;

#pragma comment(lib, "DebugToolsLibrary.lib")
