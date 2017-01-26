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
//テキストボックス用
std::string text = "接続していないよ";

//ジャッジ
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
		return "あなたの負け";
	case 0:
		return "ひきわけ";
	case 1:
		return "あなたの勝ち";
	}

	return "不正な結果";
}

void init()
{
	myResult = 0;
	myIsPlay = 0;
	myReTry = 0;
}

//ダイアログボックスの処理
BOOL CALLBACK MyDlgProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	//チャットクライアント
	static ChatClientThread chatClient(1);
	//タイマＩＤ
	const static UINT ID_TIMER = 1;
	//ポート番号
	int port;

	switch (msg) {
	case WM_INITDIALOG:	//ダイアログの初期化メッセージ
		//メッセージ表示テキストボックスの初期化
		SendMessage(
			GetDlgItem(hDlg, IDC_EDIT_MESSAGE),
			EM_LIMITTEXT,
			(WPARAM)64 * 1024,
			0
		);

		//送信メッセージテキストボックスの初期化
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
		//接続してないことを伝える
		SetDlgItemText(hDlg, IDC_EDIT_IS_CONNECT, "接続してません");
		//もう一回ボタンを押せないようにする
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RETRY), FALSE);

		//200ミリ秒毎のタイマイベントを設定する
		SetTimer(hDlg, ID_TIMER, 200, NULL);
		break;

	case WM_COMMAND: //ボタンが押された場合の処理
		switch (LOWORD(wp)) {
		case IDOK: //ＯＫボタンが押された
			break;
		case IDCANCEL: //キャンセルボタンが押された
		case IDC_BUTTON_END: //終了する
							 //タイマーを停止する
			KillTimer(hDlg, ID_TIMER);
			//サーバーに終了メッセージを送信
			chatClient.sendMessage("-2");
			//チャットクライアントの強制終了
			chatClient.end();
			//ダイアログボックスを閉じる
			EndDialog(hDlg, LOWORD(wp));
			break;
		case IDC_BUTTON_CONNECT: //接続ボタンが押された
								 //アドレスを取得
			char address[256];
			GetDlgItemText(hDlg, IDC_EDIT_SERVER, address, sizeof(address));
			//ポート番号を取得
			int flag;
			port = GetDlgItemInt(hDlg, IDC_EDIT_PORT, &flag, FALSE);
			//ユーザー名を取得する
			char name[256];
			GetDlgItemText(hDlg, IDC_EDIT_ID, name, sizeof(name));
			//サーバーのアドレスを設定
			chatClient.setName(name);
			//サーバーのアドレスを設定
			chatClient.setServerAddress(InetSocketAddress(std::string(address), port));
			//チャットクライアントを開始する
			chatClient.start();
			//接続ボタンを押せなくする
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_CONNECT), FALSE);
			//接続が完了したら知らせる
			if (chatClient.isConnect() == true) {
				SetDlgItemText(hDlg, IDC_EDIT_IS_CONNECT, "接続しました！");
				text = "接続できたよ";
			}
			break;
		case IDC_BUTTON_ROCK: //グーボタンが押されたら
			myResult = 0;
			myIsPlay = 1;
			break;
		case IDC_BUTTON_SCISSORS: //チョキボタンが押されたら
			myResult = 1;
			myIsPlay = 1;
			break;
		case IDC_BUTTON_PAPER: //パーボタンが押されたら
			myResult = 2;
			myIsPlay = 1;
			break;
		case IDC_BUTTON_RETRY:
			myReTry = 1;
		}
	case WM_TIMER:
		SetDlgItemText(hDlg, IDC_EDIT_MESSAGE, text.c_str());
		//接続していなければ何もしない
		if (chatClient.isConnect() == false) {
			break;
		}

		chatClient.sendMessage(std::to_string(myResult) + " " + std::to_string(myIsPlay) + " " + std::to_string(myReTry));

		if (chatClient.connectNum() < 2) {
			text = "人数が足りてないよ"; 
			init();
			break;
		}

		//パラメーター用文字列ストリーム
		std::stringstream ssParamater;
		//パラメーター
		std::string paramater;
		//パラメータバッファの内容を文字列ストリームに出力
		chatClient.printMessage(ssParamater);

		int connectNum;
		std::string name;
		int result;
		int isPlay;
		int retry;

		ssParamater >> connectNum >> name >> result >> isPlay >> retry;

		//自分が何もしていないなら
		if (myIsPlay == 0) {
			text = "自分が決めてないよ";
			break;
		}

		//各ボタンを押せなくする
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_ROCK), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_SCISSORS), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_PAPER), FALSE);


		//相手が何もしていないなら何もしない
		if (isPlay == 0) {
			text = "相手を待っています";
			break;
		}

		//じゃんけんの結果を返す
		text = juge(myResult, result) + "：もう一回やる？";
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_RETRY), TRUE);

		if (myReTry == 0) break;

		text = "相手がリトライするのを待ってるよ";

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

//ウィンドウのメイン関数
int WINAPI WinMain(HINSTANCE hCurInst, HINSTANCE hPrelnst, LPSTR, int nCmdShow)
{
	//WinSockの初期化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	//ダイアログボックスを開く
	DialogBox(hCurInst, (LPSTR)IDD_CLIENT, NULL, (DLGPROC)MyDlgProc);

	//WinSockの終了処理
	WSACleanup();

	return 0;
}
