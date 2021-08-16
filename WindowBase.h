#pragma once

#include<Windows.h>
#include<tchar.h>

class WindowFactory
{
//�����o�ϐ�
protected:

	WNDCLASSEX		_wndClass;	//�E�B���h�E�̊�{�ݒ��ێ�����\����
	CREATESTRUCT	_cs;		//�E�B���h�E�쐬�ɕK�v�ȃp�����[�^��ێ�����\����
	HWND			_hwnd;		//�E�B���h�E�n���h��(���E�B���h�E�I�u�W�F�N�g)

//�R���X�g���N�^�A�f�X�g���N�^
protected:

	WindowFactory();
	virtual ~WindowFactory();

	WindowFactory(const WindowFactory&) = delete;
	WindowFactory& operator=(const WindowFactory&) = delete;

//�������֘A
protected:

	//�E�B���h�E�̐ݒ肩��쐬�܂ł��s���A���\�����Ă����v�ȏ�Ԃɂ���B
	bool Init(LPCTSTR className, int posX = CW_USEDEFAULT, int posY = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);
	
	//�ݒ肵���E�B���h�E�̌����ڂ�v���V�[�W����o�^����B
	bool RegisterWindowClass(LPCTSTR className);

	//�o�^�����E�B���h�E�̐ݒ�ŃE�B���h�E���쐬�B
	bool CreateMainWindow(HINSTANCE hInstance, LPCTSTR className);

//�E�B���h�E�v���V�[�W��
protected:
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
private:
	static	LRESULT GlobalWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

//��Еt��
protected:

	//�f�X�g���N�^���Ă΂��O�ɂ���Ă���������������Ȃ�
	virtual bool Release() const;

//�A�N�Z�b�T
public:

	WNDCLASSEX		GetWndClass() const;
	CREATESTRUCT	GetCreateStruct() const;

	// NOTE : �E�B���h�E�n���h���̓|�C���^�ł��邱�Ƃɒ��ӁB
	HWND			GetHWND() const;

	void			SetWindowClass(const WNDCLASSEX& wndClass);
	void			SetCreateStruct(const CREATESTRUCT& cs);
};

class WindowCommander
{
//�E�B���h�E�̂��d������
public:

	//���������ɂ���Ă�����������
	virtual bool Begin(
		LPCTSTR className, 
		int		posX   = CW_USEDEFAULT, 
		int		posY   = CW_USEDEFAULT, 
		int		width  = CW_USEDEFAULT, 
		int		height = CW_USEDEFAULT
	) = 0;

//�E�B���h�E�̂��d�����H
public:

	//�E�B���h�E�̕\��
	void Show(HWND hwnd) const;
	//WM_PAINT���b�Z�[�W�̑��M
	bool Update(HWND hwnd) const;

	//�I�[�o�[���C�h���Ē��g���e���������Ă��������B
	virtual void MainLoop() = 0;

	//���[�v�����̃T���v��
	void SampleMainLoop(HWND hwnd);
	//�Q�[���̃��C�����[�v������
	void SampleGameLoop(HWND hwnd);

//�E�B���h�E�̂��d���I��
public:

	//����������ɂ���Ă�����������
	virtual bool End() = 0;
};

class WindowBase : public WindowFactory, public WindowCommander
{
public:

	bool Begin(
		LPCTSTR className, 
		int		posX   = CW_USEDEFAULT, 
		int		posY   = CW_USEDEFAULT, 
		int		width  = CW_USEDEFAULT, 
		int		height = CW_USEDEFAULT
	);

	void Show() const;
	bool Update() const;

	void MainLoop();

	bool End();
};

#include"WindowBase_Impl.h"
