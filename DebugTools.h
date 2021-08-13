#pragma once

#include<tchar.h>	//�V���O�����A�}���`�o�C�g�������A�����Ŕ��f����}�N������������B
#include<Windows.h>	//OutputDebugString�}�N��
#include<crtdbg.h>	//_ASSERT_EXPR�}�N��

//�֗��ȃf�o�b�O�@�\���A�ϒ�������������悤�Ɋg���B
#ifdef _DEBUG

	//�ϒ�������������OutputDebugString�}�N��
	#ifndef OutputDebugStringEx
		#define OutputDebugStringEx(str, ...)\
				{											\
					TCHAR tstr[512];						\
					_stprintf_s(tstr, str, __VA_ARGS__);	\
					OutputDebugString(tstr);				\
				}
	#endif

	//�ϒ�������������_ASSERT_EXPR�}�N��
	#ifndef _ASSERT_EXPR_EX
		#define _ASSERT_EXPR_EX(expr, str, ...)\
				{											\
					TCHAR tstr[256];						\
					_stprintf_s(tstr, str, __VA_ARGS__);	\
					_ASSERT_EXPR(expr, tstr);				\
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

// WindowsAPI�̒��߂̃G���[�R�[�h���E�B���h�E��ʂŐ������A���̃G���[�R�[�h�Ńv���O�������I������B
// NOTE : �f�o�b�O���[�h�łȂ��Ȃ�A���̊֐��͂Ȃɂ����Ȃ��B
void ErrorExit(LPTSTR lpszFunction);
