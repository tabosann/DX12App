#pragma once

#include"WindowBase.h"
#include"Dx12Base.h"
#include<iostream>

class Dx12App : public wbl::WindowBase
{
// �O���Ŏg�p����֐�
public:

	// �B��̎��̂𐶐����A�����Ԃ��B
	static Dx12App& Instance();

	// �v���O�����̋N���iBegin�֐���End�֐��̊Ԃɂ�邱�Ɓj
	void			Run();

	// �B��̎��̂��������
	// NOTE : ��x�������ƁA��x�Ɠ������̂��g���Ȃ��Ȃ�̂Œ���
	static void		Release();

// �O���Ŏg�p���Ȃ��֐�
private:

	// ���������E�B���h�E�̓���������Ō��߂�
	virtual LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// �����Ɏ�������肽�����Ƃ��v���O��������
	// e.g. �`��AGPU�ɂ��v�Z�Ȃ�
	virtual void	MainLoop();

	// �R���X�g���N�^
	// NOTE : �V���O���g���݌v�̂��߁AInstance�֐��ł̂ݎg�p���邱��
	Dx12App() : wbl::WindowBase() {}

	// �f�X�g���N�^
	// NOTE : Release�֐��ł̂ݎg�p����idelete���Z�q�ŌĂяo���j����
	~Dx12App() { delete _pDx12Base; }

	// �B��̎��̂ւ̃|�C���^
	static Dx12App* _pInstance;

	// Dx12�̏�������S���i�����ɍ쐬�ς̃E�B���h�E�n���h����n�����Ɓj
	Dx12Base*		_pDx12Base;
};
