#pragma once

#include"WindowBase.h"
#include"Dx12Base.h"
#include<iostream>

class Dx12App : public wbl::WindowBase
{
// ���[�U��`�֐�
private:

	// ���������E�B���h�E�̓���������Ō��߂�
	virtual LRESULT	LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	// �����Ɏ�������肽�����Ƃ��v���O��������
	// e.g. �`��AGPU�ɂ��v�Z�Ȃ�
	virtual void	MainLoop();

public:

	// �B��̎��̂𐶐����A�����Ԃ��B
	static Dx12App& Instance();

	// �v���O�����̋N���iBegin�֐���End�֐��̊Ԃɂ�邱�Ɓj
	void			Run();

	// �B��̎��̂��������
	// NOTE : ��x�������ƁA��x�Ɠ������̂��g���Ȃ��Ȃ�̂Œ���
	static void		Release();

private:

	// �R���X�g���N�^
	// NOTE : �V���O���g���݌v�̂��߁AInstance�֐��ł̂ݎg�p���邱��
	Dx12App() : wbl::WindowBase(), _pDx12Base(nullptr) {}

	// �f�X�g���N�^
	// NOTE : Release�֐��ł̂ݎg�p����idelete���Z�q�ŌĂяo���j����
	~Dx12App() { delete _pDx12Base; }

	// ���[�U��`�̃^�X�N���ʔԍ�
	struct UserTaskID : Dx12Base::TaskID
	{
		using TaskID::TaskID;
		enum _TaskID
		{
			IMGUI_DX12_INIT = LAST_TASK_ID + 1,
			IMGUI_DX12_BEGIN,
			IMGUI_DX12_UPDATE,
			IMGUI_DX12_END,
			LAST_USER_TASK_ID
		};
	};

private:

	// �B��̎��̂ւ̃|�C���^
	static Dx12App* _pInstance;

	// Dx12�̏�������S���i�����ɍ쐬�ς̃E�B���h�E�n���h����n�����Ɓj
	Dx12Base*		_pDx12Base;
};

// �^�X�N�N���X�ɓn�������p�����[�^�����[�U�������Ŏ�������
class Dx12UserComponent : public Dx12Component
{
public:

	using Dx12Component::Dx12Component;

	FLOAT	_backGroundColor[4];	// RGBA

	// TODO : �����̂̒��_�o�b�t�@�A�C���f�b�N�X�o�b�t�@
};
