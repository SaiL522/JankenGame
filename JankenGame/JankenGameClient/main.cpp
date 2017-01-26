#include <WinSock2.h>
#include <Windows.h>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "resource.h"
#include "ChatClientThread.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "NetLibrary.lib")
#pragma comment(lib, "ThreadLibrary.lib")

int myResult = 0;
int myIsPlay = 0;
int myReTry = 0;
//�e�L�X�g�{�b�N�X�p
std::string text = "�ڑ����Ă��Ȃ���";

//�W���b�W
std::string juge(int my, int other)
{
	int results[3][3] = {
		{0,1,-1},
		{-1,0,1},
		{1,-1,0}
	};

	switch (results[my][other])
	{
	case -1:
		return "���Ȃ��̕���";
	case 0:
		return "�Ђ��킯";
	case 1:
		return "���Ȃ��̏���";
	}

	return "�s���Ȍ���";
}

void init()
{
	myResult = 0;
	myIsPlay = 0;
	myReTry = 0;
}

//�_�C�A���O�{�b�N�X�̏���
BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	//�`���b�g�N���C�A���g
	static ChatClientThread chatClient(1);
	//�^�C�}�h�c
	const static UINT ID_TIMER = 1;
	//�|�[�g�ԍ�
	int port;

	switch (msg) {
	case WM_INITDIALOG:	//�_�C�A���O�̏��������b�Z�[�W
		//���b�Z�[�W�\���e�L�X�g�{�b�N�X�̏�����
		SendMessage(
			GetDlgItem(hDlg, IDC_EDIT_MESSAGE),
			EM_LIMITTEXT,
			(WPARAM)64 * 1024,
			0
		);

		//���M���b�Z�[�W�e�L�X�g�{�b�N�X�̏�����
		SendMessage(
			GetDlgItem(hDlg, IDC_EDIT_SEND),
			EM_LIMITTEXT,
			(WPARAM)1024,
			0
		);

		SendMessage(
			GetDlgItem(hDlg, IDC_EDIT_IS_CONNECT),
			EM_LIMITTEXT,
			(WPARAM)1024,
			0
		); 
		//�ڑ����ĂȂ����Ƃ�`����
		SetDlgItemText(hDlg, IDC_EDIT_IS_CONNECT, "�ڑ����Ă܂���");
		//�������{�^���������Ȃ��悤�ɂ���
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RETRY), FALSE);

		//200�~���b���̃^�C�}�C�x���g��ݒ肷��
		SetTimer(hDlg, ID_TIMER, 200, NULL);
		break;

	case WM_COMMAND: //�{�^���������ꂽ�ꍇ�̏���
		switch (LOWORD(wp)) {
		case IDOK: //�n�j�{�^���������ꂽ
			break;
		case IDCANCEL: //�L�����Z���{�^���������ꂽ
		case IDC_BUTTON_END: //�I������
							 //�^�C�}�[���~����
			KillTimer(hDlg, ID_TIMER);
			//�T�[�o�[�ɏI�����b�Z�[�W�𑗐M
			chatClient.sendMessage("-2");
			//�`���b�g�N���C�A���g�̋����I��
			chatClient.end();
			//�_�C�A���O�{�b�N�X�����
			EndDialog(hDlg, LOWORD(wp));
			break;
		case IDC_BUTTON_CONNECT: //�ڑ��{�^���������ꂽ
								 //�A�h���X���擾
			char address[256];
			GetDlgItemText(hDlg, IDC_EDIT_SERVER, address, sizeof(address));
			//�|�[�g�ԍ����擾
			int flag;
			port = GetDlgItemInt(hDlg, IDC_EDIT_PORT, &flag, FALSE);
			//���[�U�[�����擾����
			char name[256];
			GetDlgItemText(hDlg, IDC_EDIT_ID, name, sizeof(name));
			//�T�[�o�[�̃A�h���X��ݒ�
			chatClient.setName(name);
			//�T�[�o�[�̃A�h���X��ݒ�
			chatClient.setServerAddress(InetSocketAddress(std::string(address), port));
			//�`���b�g�N���C�A���g���J�n����
			chatClient.start();
			//�ڑ��{�^���������Ȃ�����
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), FALSE);
			//�ڑ�������������m�点��
			if (chatClient.isConnect() == true) {
				SetDlgItemText(hDlg, IDC_EDIT_IS_CONNECT, "�ڑ����܂����I");
				text = "�ڑ��ł�����";
			}
			break;
		case IDC_BUTTON_ROCK: //�O�[�{�^���������ꂽ��
			myResult = 0;
			myIsPlay = 1;
			break;
		case IDC_BUTTON_SCISSORS: //�`���L�{�^���������ꂽ��
			myResult = 1;
			myIsPlay = 1;
			break;
		case IDC_BUTTON_PAPER: //�p�[�{�^���������ꂽ��
			myResult = 2;
			myIsPlay = 1;
			break;
		case IDC_BUTTON_RETRY:
			myReTry = 1;
		}
	case WM_TIMER:
		SetDlgItemText(hDlg, IDC_EDIT_MESSAGE, text.c_str());
		//�ڑ����Ă��Ȃ���Ή������Ȃ�
		if (chatClient.isConnect() == false) {
			break;
		}

		chatClient.sendMessage(std::to_string(myResult) + " " + std::to_string(myIsPlay) + " " + std::to_string(myReTry));

		if (chatClient.connectNum() < 2) {
			text = "�l��������ĂȂ���"; 
			init();
			break;
		}

		//�p�����[�^�[�p������X�g���[��
		std::stringstream ssParamater;
		//�p�����[�^�[
		std::string paramater;
		//�p�����[�^�o�b�t�@�̓��e�𕶎���X�g���[���ɏo��
		chatClient.printMessage(ssParamater);

		int connectNum;
		std::string name;
		int result;
		int isPlay;
		int retry;

		ssParamater >> connectNum >> name >> result >> isPlay >> retry;

		//�������������Ă��Ȃ��Ȃ�
		if (myIsPlay == 0) {
			text = "���������߂ĂȂ���";
			break;
		}

		//�e�{�^���������Ȃ�����
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_ROCK), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SCISSORS), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_PAPER), FALSE);


		//���肪�������Ă��Ȃ��Ȃ牽�����Ȃ�
		if (isPlay == 0) {
			text = "�����҂��Ă��܂�";
			break;
		}

		//����񂯂�̌��ʂ�Ԃ�
		text = juge(myResult, result) + "�F���������H";
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RETRY), TRUE);

		if (myReTry == 0) break;

		text = "���肪���g���C����̂�҂��Ă��";

		if (retry == 0) break;

		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RETRY), FALSE);
		init();
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_ROCK), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SCISSORS), TRUE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_PAPER), TRUE);

		break;
	}
	return FALSE;
}

//�E�B���h�E�̃��C���֐�
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrelnst, LPSTR, int nCmdShow)
{
	//WinSock�̏�����
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	//�_�C�A���O�{�b�N�X���J��
	DialogBox(hCurInst, (LPSTR)IDD_CLIENT, NULL, (DLGPROC)MyDlgProc);

	//WinSock�̏I������
	WSACleanup();

	return 0;
}
